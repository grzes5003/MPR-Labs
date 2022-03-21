import itertools
from dataclasses import dataclass
import seaborn as sns
import pandas as pd
from matplotlib import pyplot as plt


@dataclass
class Rec:
    msg_size: int
    dur: float
    variant: int

    @classmethod
    def from_str(cls, _input: str, variant: int):
        items = _input.split(':')
        return cls(msg_size=int(items[0]), dur=float(items[2]), variant=variant)

    def mbps(self):
        return self.msg_size * 8 / self.dur if self.dur != 0 else 0


def read_logs(path: str) -> [Rec]:
    _start_char = '#'

    with open(path, 'r') as f:
        lines = f.readlines()
    lines = list(itertools.dropwhile(lambda line: line[0] != _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    size = len(lines)
    idx_list = [idx + 1 for idx, val in enumerate(lines) if val[0] == _start_char]
    chunks = [lines[i - 1: j - 1] for i, j in zip([0] + idx_list, idx_list + ([size] if idx_list[-1] != size else []))]

    return [[Rec.from_str(it, int(chunk[0][-1])) for it in chunk[1:]] for chunk in chunks if len(chunk) != 0]


def obj2df(results: [Rec]) -> pd.DataFrame:
    return pd.DataFrame([[res.msg_size, res.dur, res.variant, res.mbps()] for res in results],
                        columns=['msg_size', 'dur', 'variant', 'mbps'])


def plot_lab01(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    # sns.lineplot(x='msg_size', y='mbps', data=df[df.variant == 0], linewidth=0.7)
    # ax = sns.lineplot(x='msg_size', y='mbps', data=df[df.variant == 1], linewidth=0.7)
    ax = sns.lineplot(x='msg_size', y='mbps', data=df, hue='variant', linewidth=0.7)
    # ax.set(xlim=(0, 40000))
    plt.legend(labels=["send", "ssend"])
    plt.ylabel("Throughput [Mbps]")
    ax.set_title('Plot of thp versus message size for smaller messages')
    plt.xlabel("Message size [Bytes]")
    plt.show()


def plot_throughput(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    ax = sns.lineplot(x='msg_size', y='mbps', data=df, hue='variant', linewidth=0.7)
    plt.ylabel("Throughput [Mbps]")
    plt.xlabel("Message size [Bytes]")
    # plt.ticklabel_format(style='plain', axis='y')
    ax.set_title('Plot of throughput versus message size for multicore processor')
    ax.legend(labels=['MPI_send', 'MPI_Ssend'])
    ax.set(xscale="log")
    plt.show()


if __name__ == '__main__':
    # path1 = '../results/lab01to2/log_20_08_15.log'
    # path2 = '../results/lab01to2/log_20_14_07.log'
    # res = [*read_logs(path1)[0], *read_logs(path2)[0]]
    # path = '../results/lab01to2/log_21_19_39.log'    # big nums remote
    # path = '../results/lab01to2/log_21_23_35.log'    # big nums local
    # path = '../results/lab01to2/log_21_20_56.log'   # small nums remote
    path = '../results/lab01to2/log_22_00_01.log'   # small nums local
    res = read_logs(path)
    df = obj2df([*res[0], *res[1]])
    plot_lab01(df)
