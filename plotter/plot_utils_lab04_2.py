from dataclasses import dataclass
import matplotlib.pyplot as plt
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
    times = [
        't_rand',
        't_bck',
        't_sort',
        't_cpy',
        't_clean'
    ]

    for alg in df['alg'].unique():
        for n_buckets in df['b'].unique():
            sns.set_theme(style="darkgrid")
            # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
            # ax = sns.pointplot(x="threads", y="t_all", data=df[(df['alg'] == alg) & (df['b'] == n_buckets)],
            #                    hue='array', legend=True, ci='sd')

            sns.set_theme(style="darkgrid")
            # ax = sns.lineplot(x=range(1, 13), y=np.repeat(1, 12)/range(1, 13), linestyle='--', lw=1)
            ax = sns.pointplot(x="threads", y="t_all",
                               data=df[(df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
                               ci='sd', hue='array')
            # sns.pointplot(x="threads", y="t_bck",
            #               data=df[(df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
            #               ci='sd', ax=ax, color='r')
            # sns.pointplot(x="threads", y="t_sort",
            #               data=df[(df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
            #               ci='sd', ax=ax, color='g')
            # sns.pointplot(x="threads", y="t_cpy",
            #               data=df[(df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
            #               ci='sd', ax=ax, color='y')
            # sns.pointplot(x="threads", y="t_clean",
            #               data=df[(df['b'] == n_buckets) & (df['alg'] == alg)], legend=True,
            #               ci='sd', ax=ax, color='black')
            ax.set(ylabel='Duration [s]')
            ax.set_title(f'Duration based on used threads {n_buckets=}, {alg=}')
            ax.set(xlabel='Number of threads')

        # custom_lines = [Line2D([0], [0], color='b', lw=4),
        #                 Line2D([0], [0], color='r', lw=4),
        #                 Line2D([0], [0], color='g', lw=4),
        #                 Line2D([0], [0], color='y', lw=4),
        #                 Line2D([0], [0], color='black', lw=4)]
        # ax.legend(custom_lines, ["All", 'Bucket split', 'sort', 'copying time', 'clean up'])
        plt.show()

            # ax.set(yscale="log")
            # ax.legend(title='Number of points [n]')
            # ax.set(ylabel='Duration [s]')
            # ax.set_title(f'Duration based on used threads')
            # ax.set(xlabel='Number of threads')
            # plt.show()


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
            plt.show()


def plot_speedup(df: pd.DataFrame):
    val = 120000000
    times = [
        't_rand',
        't_bck',
        't_sort',
        't_cpy',
        't_clean'
    ]

    colors = ['b', 'r', 'g', 'y', 'black']

    df = df[(df['array'] == val)]
    for alg in df['alg'].unique():
        if alg != 1:
            continue
        for n_buckets in df['b'].unique():
            if n_buckets != 120000:
                continue
            # df_out = df.groupby('cores').mean()
            # std = df.groupby('cores').std()
            t1 = df[(df['alg'] == alg) & (df['b'] == n_buckets) & (df['threads'] == 1)].mean()

            for time in times:
                df[f'speedup_{time}'] = t1[time] / df[time]

            sns.set_theme(style="darkgrid")
            for time, color in zip(times, colors):
                sns.lineplot(x=[0, df[(df['array'] == val) & (df['b'] == n_buckets)
                                      & (df['alg'] == alg)]['threads'].max() / 2],
                             y=[1, df[(df['array'] == val) & (df['b'] == n_buckets)
                                      & (df['alg'] == alg)]['threads'].max()],
                             linestyle='--', lw=1, color='b')
                # sns.lineplot(x=[0, 6],
                #              y=[1, 12],
                #              linestyle='--', lw=1, color='b')
                ax = sns.pointplot(x="threads", y=f"speedup_{time}",
                                   data=df[(df['array'] == val) & (df['b'] == n_buckets) & (df['alg'] == alg)],
                                   ci='sd', color=color)
                ax.set(ylabel='Speedup')
                ax.set_title(f'Speedup based on used threads, {alg=}, {n_buckets=}')
                ax.set(xlabel='Number of threads')
            custom_lines = [Line2D([0], [0], color='b', lw=4),
                            Line2D([0], [0], color='r', lw=4),
                            Line2D([0], [0], color='g', lw=4),
                            Line2D([0], [0], color='y', lw=4),
                            Line2D([0], [0], color='black', lw=4)]
            ax.legend(custom_lines, ["All", 'Bucket split', 'Sort', 'Copying time', 'Clean up'])
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
            num = line.rstrip().split(',')
            [nums.append(int(n)) for n in num if len(n) != 0]
    sns.displot(nums, kind="kde", bw_adjust=.25)
    plt.show()
    ...


if __name__ == "__main__":
    path = '../lab04/res/run_alg2.log'
    path2 = '../lab04/res/run_alg2_more.log'
    path3 = '../lab04/res/run_24_18.log'
    path4 = '../lab04/res/run_alg1_more.log'
    path5 = '../lab04/results.log'
    path6 = '../lab04/latest_log.log'
    # res = [*read_logs(path), *read_logs(path2), *read_logs(path3), *read_logs(path4)]
    res = [*read_logs(path6)]
    df = obj2df(res)
    # plot_second(df)
    # plot_one_thrd(df)
    # plot_speedup(df)
    # plot_chunks(df)
    # plot_lab04(df)
    plot_lab04(df)

    # path = '../lab04/cmake-build-debug/arr_t8.txt'
    # plot_dist(path)
