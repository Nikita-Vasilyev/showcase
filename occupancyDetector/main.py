import pandas as pd
import numpy as np
import os
from datetime import datetime

def euclidean_dist(x, y):
    total_sum = 0
    for i in range(len(x)):
        total_sum += (x[i] - y[i])**2
    return total_sum**0.5

def kMeans(train_data, k, max_iterations=100):
    # initialize centroids randomly
    centroids = [ [ float(train_data[tag][i]) for tag in train_data.keys() ] for i in range(k) ]
    train_data_values = [ [ float(train_data[tag][i]) for tag in train_data.keys() ] for i in range(1, len(train_data)) ]
    
    for j in range(max_iterations):
        # assign each data point to the nearest centroid
        clusters = [[] for i in range(k)]
        for point in train_data_values:
            distances = [euclidean_dist(point, centroid) for centroid in centroids]
            cluster_index = np.argmin(distances)
            clusters[cluster_index].append(point)
        
        # update centroids
        new_centroids = []
        for cluster in clusters:
            new_centroid = np.mean(cluster, axis=0)
            new_centroids.append(new_centroid)
        
        # check for convergence
        if np.allclose(centroids, new_centroids):
            break
        
        centroids = new_centroids

    # Calculate SSE
    sse = 0
    for i, centroid in enumerate(centroids):
        for point in clusters[i]:
            sse += euclidean_dist(point, centroid) ** 2
    
    return centroids, sse

def find_optimal_clusters(train_data, max_k=5, max_iterations=100):
    sse_values = []
    centroid_values = []
    for k in range(1, max_k + 1):
        try:
            centroids, sse = kMeans(train_data, k, max_iterations)
        except:
            continue
        sse_values.append(sse)
        centroid_values.append(centroids)
    
    # Calculate the rate of decrease in SSE
    sse_diff = np.diff(sse_values)
    sse_diff_ratio = sse_diff[:-1] / sse_diff[1:]
    
    # Find the index of the "elbow point"
    best_k_index = np.argmax(sse_diff_ratio)
    best_k = best_k_index + 2  # Since we calculated differences, add 2
    best_centroid = centroid_values[best_k - 1]
    
    return best_k, best_centroid

def func(x):
    return 10**(x / 10)

def main():
    # set up nic
    os.system("sudo ./nic_setup.sh")

    # packet process loop
    while True:
        # capture packets
        os.system("./run_capture.sh")

        # import csv file
        df = pd.read_csv('packets.csv')

        # set contains all processed source addresses 
        found_set = set()

        # contains the source address as the key and list (average power, # of packets)
        found_dict = dict()

        new_df = pd.DataFrame(columns=["pwr", "f(pwr)"])

        # process all packets
        for row in range(df.shape[0]):
            cur_row = list(df.iloc[row])

            # checks if a packet is a probe request
            if cur_row[2] == 4:
                if cur_row[0] in found_set:
                    # current elements
                    pwr = found_dict[cur_row[0]][0]
                    tot_packets = found_dict[cur_row[0]][1]

                    # adjust average value and number of packets
                    new_pwr = (pwr*tot_packets + int(cur_row[1])) / (tot_packets + 1)
                    new_pack = tot_packets + 1
                    found_dict.update({cur_row[0] : [new_pwr, new_pack]})
                else:
                    # add source address to set
                    found_set.add(cur_row[0])

                    # add to dict
                    found_dict.update({cur_row[0] : [int(cur_row[1]), 1]})
        
        # add pwr to new df
        for index, key in enumerate(found_dict.keys()):
            pwr = found_dict[key][0]
            new_df.loc[index] = [pwr, func(pwr)]

        # run k-means
        k, centroids = find_optimal_clusters(new_df)

        # Get the current local time
        current_time = datetime.now().time()

        # Format the time as hour:min:sec
        formatted_time = current_time.strftime("%H:%M:%S")

        with open("main_output.txt", "a") as f:
            f.write(formatted_time+" - "+str(k)+" Clusters with:\n")
            for i in range(len(centroids)):
                f.write(str(i)+": "+str(centroids[i][0])+" dBm, "+str(centroids[i][1])+" mW\n")
                
main()
