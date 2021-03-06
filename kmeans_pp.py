import os
import os.path
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from typing import List
import mykmeanssp

np.random.seed(0)


def initialize_centroids(S, k):
    N = len(S)
    i = 1
    first_idx = np.random.randint(N)
    centroids = np.array(S[first_idx]).reshape(-1, len(S[0]))
    indices = [first_idx]
    while i < k:
        D = np.zeros(N)
        for l in range(N):
            D[l] = (np.linalg.norm(S[l] - centroids, axis=1) ** 2).min()
        D_SUM = D.sum()
        Prob = D / D_SUM
        next_mu_index = np.random.choice(np.arange(N), p=Prob)
        centroids = np.append(centroids, [S[next_mu_index]], axis=0)
        indices.append(next_mu_index)
        i += 1
    return centroids, indices


def read_data(name1: str, name2: str):
    df1 = pd.read_csv(name1, sep=",", header=None)
    df2 = pd.read_csv(name2, sep=",", header=None)
    res = pd.merge(df1, df2, how='inner', on=0)
    res.sort_values(by=0, inplace=True)
    return res.drop(0, axis=1).to_numpy()


def validate_input_args(argv: List[str]) -> bool:
    n = len(argv)

    if (n != 6 and n != 5):
        return True

    k = argv[1]
    if n == 6:
        max_iter = argv[2]
        eps = argv[3]
        file_name_1, file_name_2 = argv[4], argv[5]
    else:  # n==5
        eps = argv[2]
        max_iter = "300"
        file_name_1, file_name_2 = argv[3], argv[4]

    try:
        with open(file_name_1, 'r') as file:
            pass
        with open(file_name_2, 'r') as file:
            pass
    except:
        return True

    try:
        k, max_iter, eps = float(k), float(max_iter), float(eps)
    except:
        return True

    if k != int(k) or max_iter != int(max_iter) or k <= 1 or max_iter < 1 or eps < 0:
        return True
    return False

def valid_k_vs_length(combined_inputs,k):
    N = len(combined_inputs)
    if k>N:
        return False
    return True


def get_args(argv: List[str]):
    if len(argv) == 6:
        k, max_iter, eps, file_name_1, file_name_2 = int(float(argv[1])), int(float(argv[2])), float(argv[3]), argv[4], argv[5]
    else:
        k, max_iter, eps, file_name_1, file_name_2 = int(float(argv[1])), 300, float(argv[2]), argv[3], argv[4]
    return k, max_iter, eps, file_name_1, file_name_2


def write_output(input_text, output_filename: str):
    with open(output_filename, "w") as file:
        for line in input_text:
            line_data = make_string(line)
            file.writelines(line_data)
            file.write("\n")


def make_string(centroid: List['float']):
    st = ""
    for cell in centroid:
        tmp = "%.4f" % round(cell, 4)
        st = st + tmp + ","
    return st[:len(st) - 1]

def clear_tmp_files():
    if os.path.exists("tmp_initial_centroids.txt"):
        os.remove("tmp_initial_centroids.txt")
    if os.path.exists("tmp_combined_inputs.txt"):
        os.remove("tmp_combined_inputs.txt")

def print_output(centroids,initial_centroids_indices):
    st = ""
    for centroid in centroids:
        st += make_string(centroid) + "\n";
    result = ','.join(str(ind) for ind in initial_centroids_indices)
    print(result + "\n" + st);

def main():
    argv = sys.argv
    if validate_input_args(argv):
        print("Invalid Input!")
    else:
        try:
            k, max_iter, eps, file_name_1, file_name_2 = get_args(argv)
            combined_inputs = read_data(file_name_1, file_name_2)
            if not valid_k_vs_length(combined_inputs,k):
                print("Invalid Input!")
            else:
                initial_centroids, initial_centroids_indices = initialize_centroids(combined_inputs.tolist(), k)
                write_output(initial_centroids.tolist(), "tmp_initial_centroids.txt")
                write_output(combined_inputs, "tmp_combined_inputs.txt")
                combined_path = os.path.join(os.getcwd(), "tmp_combined_inputs" + "." + "txt")
                initial_path = os.path.join(os.getcwd(), "tmp_initial_centroids" + "." + "txt")
                centroids = mykmeanssp.fit(k, max_iter, eps, combined_path, initial_path)
                clear_tmp_files()
                print_output(centroids, initial_centroids_indices)
        except:
            print("An Error Has Occurred")

if __name__ == "__main__":
    main()