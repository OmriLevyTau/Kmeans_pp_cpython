import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.cluster import KMeans
import pandas as pd
from matplotlib.patches import Ellipse


def main():
    iris = datasets.load_iris()
    k_list = list(range(1,11))
    results = {"k":[], "inertia":[]}
    for k in k_list:
        classifier = KMeans(n_clusters=k,init='k-means++',n_init=1,random_state=0)
        classifier.fit(iris.data)
        results["k"].append(k)
        results["inertia"].append(classifier.inertia_)
    df = pd.DataFrame(results)

    fig, ax = plt.subplots()
    ax.plot(df["k"].to_numpy(),df["inertia"].to_numpy())
    # adds circle and arrow
    ax.add_artist(Ellipse((3, 80), 0.7, 65, facecolor="white", edgecolor="black"))
    ax.annotate('Elbow point', xy=(3.2, 106), xycoords='data',
                xytext=(0.5, 0.3), textcoords='axes fraction',
                arrowprops=dict(facecolor='black', shrink=0.05),
                horizontalalignment='right', verticalalignment='top',)
    # titles
    plt.title("Elbow Method")
    plt.xlabel("K")
    plt.ylabel("inertia")
    plt.savefig("elbow.png")

if __name__=="__main__":
    main()





