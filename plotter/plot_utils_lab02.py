import itertools
from dataclasses import dataclass
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from matplotlib.lines import Line2D


@dataclass(repr=True)
class Pi:
    cores: int
    dur: float

    @classmethod
    def from_str(cls, _input: str):
        items = _input.split(':')
        return cls(cores=int(items[0]), dur=float(items[1]))


@dataclass(repr=True)
class Result:
    t: int
    n: int
    ests: [Pi]

    @classmethod
    def from_str(cls, _input: str, _ests: [Pi] = None):
        items = _input.split(':')
        return cls(t=int(items[1][2:]), n=int(items[2][2:]), ests=[] if _ests is None else _ests)


def read_logs(path: str) -> [Result]:
    _start_char = '#'

    with open(path, 'r') as f:
        lines = f.readlines()
    lines = list(itertools.dropwhile(lambda line: line[0] != _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    size = len(lines)
    idx_list = [idx + 1 for idx, val in enumerate(lines) if val[0] == _start_char]
    chunks = [lines[i - 1: j - 1] for i, j in zip([0] + idx_list, idx_list + ([size] if idx_list[-1] != size else []))]

    return [Result.from_str(item[0], [Pi.from_str(pi) for pi in item[1:]]) for item in chunks if len(item) != 0]


def obj2df(results: [Result]) -> pd.DataFrame:
    record = []
    for item in results:
        for est in item.ests:
            record.append([est.cores, est.dur, item.t, item.n])
    return pd.DataFrame(record, columns=['cores', 'dur', 'try', 'n'])


def plot_lab02(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    ax = sns.pointplot(x="cores", y="dur", data=df, hue='n', legend=True, ci='sd')
    ax.set(yscale="log")
    ax.legend(title='Number of points [n]')
    ax.set(ylabel='Duration [s]')
    ax.set_title('Duration based on used cores')
    ax.set(xlabel='Number of cores')
    # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
    # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
    #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
    #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
    plt.show()


def plot_speedup(df: pd.DataFrame):
    # df_out = df.groupby('cores').mean()
    # std = df.groupby('cores').std()
    t1 = df[df['cores'] == 1].groupby('n').mean()

    df['speedup'] = t1.loc[df['n']].reset_index()['dur'] / df['dur']

    # rocket = sns.color_palette("rocket")
    sns.set_theme(style="darkgrid")
    ax = sns.lineplot(x=range(0, 13), y=range(1, 14), linestyle='--', lw=1)
    sns.pointplot(x="cores", y='speedup', data=df, hue='n', ax=ax, ci="sd", lw=0.1)

    ax.set(ylabel='Speedup')
    ax.set_title('Speedup based on used cores')
    ax.set(xlabel='Number of cores')
    ax.legend(title='Number of points [n]')

    # ax.legend(['Theoretical speedup', 'Small: n=10000000', 'Medium: n='])
    plt.show()


def plot_efficiency(df: pd.DataFrame):
    t1 = df[df['cores'] == 1].groupby('n').mean()
    df['speedup'] = t1.loc[df['n']].reset_index()['dur'] / df['dur']
    df['eff'] = df['speedup'] / df['cores']

    sns.set_theme(style="darkgrid")
    ax = sns.pointplot(x='cores', y='eff', data=df, hue='n', ci="sd", capsize=.2, linewidth=0.5)

    ax.set_title('Efficiency based on used cores')
    ax.legend(title='Number of points [n]')
    ax.set(ylabel='Efficiency')
    # plt.legend(['Small: n=10000000', 'Medium: n='])
    plt.show()


def plot_sequence(df: pd.DataFrame):
    t1 = df[df['cores'] == 1].groupby('n').mean()
    df['speedup'] = t1.loc[df['n']].reset_index()['dur'] / df['dur']
    df['sf'] = (1/df['speedup'] - 1/df['cores']) / (1 - 1/df['cores'])

    sns.set_theme(style="darkgrid")
    ax = sns.pointplot(x='cores', y='sf', data=df, hue='n', ci="sd", capsize=.2, linewidth=0.5)
    ax.set_title('Sequential part based on used cores')
    ax.legend(title='Number of points [n]')
    ax.set(ylabel='Sequential factor')

    plt.show()


if __name__ == "__main__":
    path = '../results/lab01to2/log_18_00_53.log'
    path2 = '../results/lab01to2/log_21_10_41.log'
    path3 = '../results/lab01to2/log_21_15_24.log'
    res = [*read_logs(path), *read_logs(path2), *read_logs(path3)]
    df = obj2df(res)
    # plot_lab02(df)
    # plot_speedup(df)
    # plot_efficiency(df)
    plot_sequence(df)
