import itertools
from dataclasses import dataclass
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd


@dataclass(repr=True)
class Result:
    t: int
    n: int
    d: int
    c: int
    one: float
    delta: float

    @classmethod
    def from_str(cls, _input: str):
        items = _input.split(':')
        return cls(t=int(items[0][2:]), delta=float(items[1][6:]),
                   d=int(items[2][2:]), c=int(items[3][2:]),
                   n=int(items[4][2:]), one=float(items[5][4:]))


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
        record.append([item.t, item.n, item.d, item.c, item.one, item.delta])
    return pd.DataFrame(record, columns=['threads', 'num', 'dynamic', 'chunks', 'time', 'time_sum'])


def plot_lab03(df: pd.DataFrame):
    for mode in df['dynamic'].unique():
        for chunks in df['chunks'].unique():
            sns.set_theme(style="darkgrid")
            # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
            ax = sns.pointplot(x="threads", y="time", data=df[(df['dynamic'] == mode) & (df['chunks'] == chunks)],
                               hue='num', legend=True, ci='sd')
            # ax.set(yscale="log")
            ax.legend(title='Number of points [n]')
            ax.set(ylabel='Duration [s]')
            ax.set_title(f'Duration based on used threads mode={"dynamic" if mode == 2 else "static"}, {chunks=}')
            ax.set(xlabel='Number of threads')
            # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
            # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
            #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
            #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
            plt.show()


def plot02(df: pd.DataFrame):
    for mode in df['dynamic'].unique():
        sns.set_theme(style="darkgrid")
        # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
        ax = sns.pointplot(x="threads", y="time", data=df[df['dynamic'] == mode], hue='chunks', legend=True, ci='sd')
        ax.set(yscale="log")
        # ax.legend(title='Number of points [n]')
        ax.set(ylabel='Duration [s]')
        ax.set_title(f'Duration based on used threads mode={"dynamic" if mode == 2 else "static"}')
        ax.set(xlabel='Number of threads')

        plt.show()


if __name__ == "__main__":
    path = '../results/lab03/log_04_23_59.log'
    res = [*read_logs(path)]
    df = obj2df(res)
    plot_lab03(df)
    plot02(df)
