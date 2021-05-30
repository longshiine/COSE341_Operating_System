import csv
import numpy as np
import matplotlib.pyplot as plt
from pandas import Series, DataFrame

# plot function
def plot_result(values, title, legend, xlabel, ylabel, hist=False):
    for value in values:
        plt.plot(range(0,len(value)), value)
    plt.title(title)
    plt.legend(legend)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    if hist:
      plt.grid(True, alpha=0.5, linestyle='--')
    else:
      plt.gca().axes.yaxis.set_ticklabels([])
      plt.grid(True, axis='y', alpha=0.5, linestyle='--')
    plt.show()

def cal_frequency(delta_rrun):
  data = {}
  for runtime in delta_rrun:
    ms = round(runtime / 100, 1)
    if ms not in data.keys():
      data[ms] = 1
    else:
      data[ms] += 1
  return data



def print_graph(rdr, tgid, priority):
  delta_vrun = []
  delta_rrun = []
  runtime = []
  total_count = 0
  count = 0
  diff_delta = 0
  for idx,line in enumerate(rdr):
    runtime.append(int(line[5]))
    if int(line[1]) == tgid:
      delta_vrun.append(int(line[3]))
      delta_rrun.append(int(line[5]))
      total_count += 1
      if line[3] != line[5]:
        count += 1
        diff_delta += int(line[5]) - int(line[3])
  # Cumulate results
  delta_vrun = np.cumsum(delta_vrun)
  delta_rrun = np.cumsum(delta_rrun)

  # Plot results to graph
  #plot_result([delta_vrun, delta_rrun], priority+" Priority", ['cum_vrun', 'cum_exec'],"Samples", "cum_value")
  plot_result([[value for (key, value) in sorted(cal_frequency(runtime).items())]], "Histogram of CPU-burst times", [priority+'Priority'], "duration", "frequency", True)
  # Print Some Information
  print("<{} priority, PID={}>".format(priority, tgid))
  print("총 샘플 수: {}".format(total_count))
  if priority == "Normal":
    print("Virtual runtime과 CPU burst가 다른 샘플 수: {}".format(count))
    print("평균 Overhead: {}ns".format(round((diff_delta / count)/1000000, 3)))
  print("Total CPU-burst: {}".format(delta_rrun[-1]))
  print("\n")
  return delta_rrun[-1]

if __name__ == "__main__":
  
  # Read log data from csv file
  normal_read_f = open('data/log_normal.csv', 'r', encoding='utf-8')
  normal_rdr = csv.reader(normal_read_f)

  low_read_f = open('data/log_low.csv', 'r', encoding='utf-8')
  low_rdr = csv.reader(low_read_f)
  
  high_read_f = open('data/log_high.csv', 'r', encoding='utf-8')
  high_rdr = csv.reader(high_read_f)

  # define information for plot
  process_list = [
    {
      'tgid': 2231, 
      'priority': "Normal",
      'rdr': normal_rdr
    },
    {
      'tgid': 2729, 
      'priority': "Low",
      'rdr': low_rdr
    },
    {
      'tgid': 2209, 
      'priority': "High",
      'rdr': high_rdr
    }
  ]

  data = {}
  # print graphes
  for process in process_list:
    data[str(process['priority'])+' priority'] = [print_graph(process['rdr'], process['tgid'], process['priority'])]