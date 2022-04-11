import itertools
from dataclasses import dataclass
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd


@dataclass(repr=True)
class Result:
    t: int
    alg: int
    arr: int
    b: int
    time: float

    @classmethod
    def from_str(cls, _input: str):
        items = _input.split(':')
        return cls(t=int(items[0][2:]), alg=int(items[1][4:]),
                   arr=int(items[2][4:]), b=int(items[3][2:]),
                   time=float(items[4][5:]))


def read_logs(path: str) -> [Result]:
    _start_char = 't'

    with open(path, 'r') as f:
        lines = f.readlines()
    lines = list(filter(lambda line: line[0] == _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    return [Result.from_str(item) for item in lines]


def obj2df(results: [Result]) -> pd.DataFrame:
    record = []
    for item in results:
        record.append([item.t, item.alg, item.arr, item.b, item.time])
    return pd.DataFrame(record, columns=['threads', 'alg', 'array', 'bucket', 'time'])


def plot_lab03(df: pd.DataFrame):
    for n_buckets in df['bucket'].unique():
        sns.set_theme(style="darkgrid")
        # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
        ax = sns.pointplot(x="threads", y="time", data=df[(df['bucket'] == n_buckets)],
                           hue='array', legend=True, ci='sd')
        # ax.set(yscale="log")
        ax.legend(title='Number of points [n]')
        ax.set(ylabel='Duration [s]')
        ax.set_title(f'Duration based on used threads {n_buckets=}')
        ax.set(xlabel='Number of threads')
        # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
        # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
        #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
        #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
        plt.show()


if __name__ == "__main__":
    path = '../lab04/log_11_18_35.log'
    res = [*read_logs(path)]
    df = obj2df(res)
    # plot_speedup(df)
    plot_lab03(df)
    # plot_chunks(df)