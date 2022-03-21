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
        return self.msg_size * 8 / self.dur


def read_logs(path: str) -> [Rec]:
    _start_char = '#'

    with open(path, 'r') as f:
        lines = f.readlines()
    lines = list(itertools.dropwhile(lambda line: line[0] != _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    size = len(lines)
    idx_list = [idx + 1 for idx, val in enumerate(lines) if val[0] == _start_char]
    chunks = [lines[i - 1: j - 1] for i, j in zip([0] + idx_list, idx_list + ([size] if idx_list[-1] != size else []))]
    print(chunks)

    return [[Rec.from_str(it, int(chunk[0][-1])) for it in chunk[1:]] for chunk in chunks if len(chunk) != 0]


def obj2df(results: [Rec]) -> pd.DataFrame:
    return pd.DataFrame([[res.msg_size, res.dur, res.variant, res.mbps()] for res in results],
                        columns=['msg_size', 'dur', 'variant', 'mbps'])


def plot_lab01(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    sns.lineplot(x='msg_size', y='mbps', data=df[df.variant == 0], linewidth=0.7)
    sns.lineplot(x='msg_size', y='mbps', data=df[df.variant == 1], linewidth=0.7)
    plt.legend(labels=["send", "ssend"])
    plt.ylabel("Throughput [Mbps]")
    plt.xlabel("Message size [Bytes]")
    plt.show()


if __name__ == '__main__':
    path1 = '../results/lab01to2/log_20_08_15.log'
    path2 = '../results/lab01to2/log_20_14_07.log'
    res = [*read_logs(path1)[0], *read_logs(path2)[0]]
    df = obj2df(res)
    plot_lab01(df)
