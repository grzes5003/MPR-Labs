from dataclasses import dataclass
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt


@dataclass(repr=True)
class Result:
    thr: int
    n: int
    time: float
    type: str

    @classmethod
    def from_str(cls, _input: str):
        items = _input.split(':')
        return cls(type=items[0], time=float(items[1][2:]), thr=int(items[3][2:]), n=int(items[4][2:]))


def read_logs(path: str) -> [Result]:
    _start_char = 't'

    with open(path, 'r') as f:
        lines = f.readlines()
    # lines = list(filter(lambda line: line[0] == _start_char, lines))
    lines = [line.replace(' ', '').replace('\n', '') for line in lines]

    return [Result.from_str(item) for item in lines]


def obj2df(results: [Result]) -> pd.DataFrame:
    record = []
    for item in results:
        record.append([item.thr, item.n, item.type, item.time])
    return pd.DataFrame(record, columns=['threads', 'arr_size', 'type', 'time'])


def plot_lab06(df: pd.DataFrame):
    for arr_size in df['arr_size'].unique():
        sns.set_theme(style="darkgrid")
        # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
        ax = sns.pointplot(x="threads", y="time", data=df[(df['arr_size'] == arr_size)],
                           hue='type', legend=True, ci='sd')
        # ax.set(yscale="log")
        ax.legend(title='Number of points [n]')
        ax.set(ylabel='Duration [s]')
        ax.set_title(f'Duration based on used threads; {arr_size=}')
        ax.set(xlabel='Number of threads')
        # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
        # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
        #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
        #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
        plt.show()


def plot_gpu(df: pd.DataFrame):
    sns.set_theme(style="darkgrid")
    # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
    ax = sns.pointplot(x="threads", y="time", data=df[(df['type'] == 'cpu')],
                       hue='arr_size', legend=True, ci='sd')
    ax.set(yscale="log")
    ax.legend(title='Number of points [n]')
    ax.set(ylabel='Duration [s]')
    ax.set_title(f'Duration based on used threads; logarithmic scale')
    ax.set(xlabel='Number of threads')
    # ax.legend(labels=['1*10^7', '250*10^7', '1500*10^7'])
    # ax.figure.legend(handles=[Line2D([], [], marker='_', color="b", label='Small: n=10000000'),
    #                           Line2D([], [], marker='_', color="r", label='Medium: n=2500000000'),
    #                           Line2D([], [], marker='_', color="g", label='Big: n=15000000000')])
    plt.show()


if __name__ == "__main__":
    path = '../lab06/results/results_10_00-37-46.txt'
    res = [*read_logs(path)]
    df = obj2df(res)
    # plot_speedup(df)
    plot_gpu(df)
    plot_lab06(df)
    # plot_chunks(df)
