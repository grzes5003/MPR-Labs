from dataclasses import dataclass
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt


@dataclass(repr=True)
class Result:
    version: str
    n: int
    b: int
    alg: str
    time: float

    @classmethod
    def from_str(cls, _input: str):
        # gpu_2:t=0.123552:n=124:b=64
        items = _input.split(':')
        return cls(version='classic' if items[0] == 't1' else 'optimized',
                   alg='naive' if items[1] == 'gpu_1' else 'shared', time=float(items[2][2:]),
                   n=int(items[3][2:]), b=int(items[4][2:]))


@dataclass(repr=True)
class ResultImg:
    n: int
    time: float

    @classmethod
    def from_str(cls, _input: str):
        # gpu_2:t=0.123552:n=124:b=64
        items = _input.split(':')
        return cls(time=float(items[0][2:]), n=int(items[1][2:]))


def read_logs(path: str) -> [Result]:
    _start_char = '#'

    with open(path, 'r') as f:
        lines = f.readlines()
    lines = list(filter(lambda line: line[0] != _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    return [Result.from_str(item) for item in lines]


def read_logs_img(path: str) -> [Result]:
    _start_char = '#'

    with open(path, 'r') as f:
        lines = f.readlines()
    lines = list(filter(lambda line: line[0] != _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    return [ResultImg.from_str(item) for item in lines]


def obj2df(results: [Result]) -> pd.DataFrame:
    record = []
    for item in results:
        record.append([item.version, item.alg, item.time, item.n, item.b])
    return pd.DataFrame(record, columns=['version', 'alg', 'time', 'n', 'block_size'])


def obj2df_img(results: [ResultImg]) -> pd.DataFrame:
    record = []
    for item in results:
        record.append([item.time, item.n])
    return pd.DataFrame(record, columns=['time', 'n'])


def plot_lab07(df: pd.DataFrame):
    for version in df['version'].unique():
        for n in df['n'].unique():
            sns.set_theme(style="darkgrid")
            # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
            ax = sns.pointplot(x="block_size", y="time", data=df[(df['version'] == version) & (df['n'] == n)],
                               hue='alg', legend=True, ci='sd')
            # ax.set(yscale="log")
            ax.legend(title='Algorithm type')
            ax.set(ylabel='Duration [s]')
            ax.set_title(f'Duration based on used block size; transpose {version=}: {n=}')
            ax.set(xlabel='Size of block size')
            # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
            # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
            #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
            #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
            plt.show()


def cmp_shared(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
    ax = sns.pointplot(x="block_size", y="time", data=df[(df['n'] == 7936) & (df['alg'] == 'shared')],
                       hue='version', legend=True, ci='sd')
    ax.legend(title='variant')
    ax.set(ylabel='Duration [s]')
    ax.set_title(f'Duration based on used block size')
    ax.set(xlabel='Size of block size')
    plt.show()


def plot_img_scaling(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
    ax = sns.pointplot(x="n", y="time", data=df, legend=True, ci='sd')
    ax.legend(title='variant')
    ax.set(ylabel='Duration [s]')
    ax.set_title(f'Duration based on used block size')
    ax.set(xlabel='Size of block size')
    plt.show()


def plot_gpu(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
    ax = sns.pointplot(x="block_size", y="time", data=df[(df['n'] == 7936)],
                       hue='version', legend=True, ci='sd')
    # ax.set(yscale="log")
    ax.legend(title='transpose version')
    ax.set(ylabel='Duration [s]')
    ax.set_title(f'Duration based on block size')
    ax.set(xlabel='Size of block size')
    # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
    # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
    #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
    #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
    plt.show()


if __name__ == "__main__":
    path = '../lab07/results/results_transpose2_13_22-13-14.txt'
    path2 = '../lab07/results/results_transpose_13_22-13-14.txt'

    path3 = '../lab07/results/results_scaling_13_23-37-19.txt'

    res = [*read_logs(path), *read_logs(path2)]
    res2 = [*read_logs_img(path3)]

    df = obj2df(res)
    df2 = obj2df_img(res2)
    ...
    plot_img_scaling(df2)
    cmp_shared(df)
    # plot_speedup(df)
    # plot_gpu(df)
    plot_lab07(df)
    # plot_chunks(df)
