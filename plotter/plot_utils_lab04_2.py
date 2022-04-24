import itertools
from dataclasses import dataclass
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd
from matplotlib.lines import Line2D


@dataclass(repr=True)
class Result:
    t_rand: float
    t_bck: float
    t_sort: float
    t_cpy: float
    t_clean: float
    t: int
    alg: int
    arr: int
    b: int
    t_all: float

    @classmethod
    def from_str(cls, _input: str):
        items = _input.split(':')
        return cls(
            t_rand=float(items[0][7:]),
            t_bck=float(items[1][6:]),
            t_sort=float(items[2][7:]),
            t_cpy=float(items[3][6:]),
            t_clean=float(items[4][8:]),
            t=int(items[5][2:]),
            alg=int(items[6][4:]),
            arr=int(items[7][4:]),
            b=int(items[8][2:]),
            t_all=float(items[9][6:])
        )


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
        record.append([
            item.t_rand,
            item.t_bck,
            item.t_sort,
            item.t_cpy,
            item.t_clean,
            item.t,
            item.alg,
            item.arr,
            item.b,
            item.t_all,
        ])
    return pd.DataFrame(record, columns=[
        't_rand',
        't_bck',
        't_sort',
        't_cpy',
        't_clean',
        'threads',
        'alg',
        'array',
        'b',
        't_all',
    ])


def plot_lab04(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
    ax = sns.pointplot(x="threads", y="t_all", data=df,
                       hue='array', legend=True, ci='sd')
    ax.set(yscale="log")
    ax.legend(title='Number of points [n]')
    ax.set(ylabel='Duration [s]')
    ax.set_title(f'Duration based on used threads')
    ax.set(xlabel='Number of threads')
    # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
    # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
    #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
    #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
    plt.show()


def plot_second(df):
    val = 120000000
    for alg in df['alg'].unique():
        for n_buckets in df['b'].unique():
            sns.set_theme(style="darkgrid")
            # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
            ax = sns.pointplot(x="threads", y="t_all",
                               data=df[(df['array'] == val) & (df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
                               ci='sd')
            sns.pointplot(x="threads", y="t_bck",
                          data=df[(df['array'] == val) & (df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
                          ci='sd', ax=ax, color='r')
            sns.pointplot(x="threads", y="t_sort",
                          data=df[(df['array'] == val) & (df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
                          ci='sd', ax=ax, color='g')
            sns.pointplot(x="threads", y="t_cpy",
                          data=df[(df['array'] == val) & (df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
                          ci='sd', ax=ax, color='y')
            sns.pointplot(x="threads", y="t_clean",
                          data=df[(df['array'] == val) & (df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
                          ci='sd', ax=ax, color='black')
            ax.set(ylabel='Duration [s]')
            ax.set_title(f'Duration based on used threads {n_buckets=}, {alg=}')
            ax.set(xlabel='Number of threads')

            custom_lines = [Line2D([0], [0], color='b', lw=4),
                            Line2D([0], [0], color='r', lw=4),
                            Line2D([0], [0], color='g', lw=4),
                            Line2D([0], [0], color='y', lw=4),
                            Line2D([0], [0], color='black', lw=4)]
            ax.legend(custom_lines, ["All", 'Bucket split', 'sort', 'copying time', 'clean up'])
            # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
            # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
            #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
            #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
            plt.show()


def plot_one_thrd(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    ax = sns.pointplot(x="b", y="t_all", data=df[(df['threads'] == 1)],
                       hue='alg', legend=True, ci='sd')
    ax.set(ylabel='Duration [s]')
    ax.set_title(f'Duration based on used buckets')
    ax.set(xlabel='Number of buckets')
    plt.show()


def plot_dist(path: str):
    nums = []
    with open(path, "r") as file:
        for line in file.readlines():
            num = line.rstrip().split(',') #using rstrip to remove the \n
            [nums.append(int(n))for n in num if len(n) != 0 ]
    sns.displot(nums, kind="kde", bw_adjust=.25)
    plt.show()
    ...


if __name__ == "__main__":
    # path = '../lab04/res/run_alg2.log'
    # res = [*read_logs(path)]
    # df = obj2df(res)
    # plot_second(df)
    # plot_one_thrd(df)
    # plot_speedup(df)
    # plot_chunks(df)

    path = '../lab04/cmake-build-debug/arr_t8.txt'
    plot_dist(path)
