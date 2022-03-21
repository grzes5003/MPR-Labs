import itertools
from dataclasses import dataclass
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd


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
    print(chunks)

    return [Result.from_str(item[0], [Pi.from_str(pi) for pi in item[1:]]) for item in chunks if len(item) != 0]


def obj2df(results: [Result]) -> pd.DataFrame:
    record = []
    for item in results:
        for est in item.ests:
            record.append([est.cores, est.dur, item.t, item.n])
    return pd.DataFrame(record, columns=['cores', 'dur', 'try', 'n'])


def plot_lab02(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    sns.pointplot(x="cores", y="dur", data=df, capsize=.2)
    plt.show()


def plot_speedup(df: pd.DataFrame):
    df = df.groupby('cores').mean()
    sns.color_palette("rocket")
    sns.set_theme(style="darkgrid")
    sns.lineplot(x=range(0, 13), y=range(1, 14), linestyle='--')
    sns.pointplot(x=df.index.values, y=df.loc[1, 'dur'] / df['dur'])
    plt.legend(['Theoretical speedup', 'Small: n=10000000', 'Medium: n='])
    plt.show()


def plot_efficiency(df: pd.DataFrame):
    df = df.groupby('cores').mean()
    sns.set_theme(style="darkgrid")
    sns.pointplot(x=df.index.values, y=(df.loc[1, 'dur'] / df['dur']) / df.index.values)
    plt.legend(['Small: n=10000000', 'Medium: n='])
    plt.show()


if __name__ == "__main__":
    path = '../results/lab01to2/log_18_00_53.log'
    res = read_logs(path)
    df = obj2df(res)
    # plot_lab02(df)
    plot_speedup(df)
    # plot_efficiency(df)
