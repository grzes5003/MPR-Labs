import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

prices = {'18 vcores (m4.large)': 0.1 * 18,
          '8 vcores (m4.large)': 0.1 * 8,
          '8 vcores (m5.xlarge)': 0.195 * 8,
          '16 vcores (m5.xlarge)': 0.195 * 16,
          'sequential (m4.large)': 0.1 * 1,
          'sequential (m5.large)': 0.195 * 1}

# m4.large 1
# m4.large 4
# m4.large 4
a = pd.DataFrame([
    {'size': 1, 'time': 4.43, 'alg': 'hadoop', 'cores': '18 vcores (m4.large)'},
    {'size': 1, 'time': 4.8, 'alg': 'hadoop', 'cores': '18 vcores (m4.large)'},
    {'size': 5, 'time': 15, 'alg': 'hadoop', 'cores': '18 vcores (m4.large)'},
    {'size': 5, 'time': 23, 'alg': 'hadoop', 'cores': '18 vcores (m4.large)'},
    {'size': 10, 'time': 31, 'alg': 'hadoop', 'cores': '18 vcores (m4.large)'},
])

# m4.large 1
# m4.large 3
b = pd.DataFrame([
    {'size': 1, 'time': 7.9, 'alg': 'hadoop', 'cores': '8 vcores (m4.large)'},
    {'size': 5, 'time': 37, 'alg': 'hadoop', 'cores': '8 vcores (m4.large)'},
    {'size': 5, 'time': 37, 'alg': 'hadoop', 'cores': '8 vcores (m4.large)'},
    {'size': 10, 'time': 78, 'alg': 'hadoop', 'cores': '8 vcores (m4.large)'},
])

# m5.xlarge 1
# m5.xlarge 1
c = pd.DataFrame([
    {'size': 1, 'time': 8, 'alg': 'hadoop', 'cores': '8 vcores (m5.xlarge)'},
    {'size': 5, 'time': 41, 'alg': 'hadoop', 'cores': '8 vcores (m5.xlarge)'},
    {'size': 10, 'time': 83, 'alg': 'hadoop', 'cores': '8 vcores (m5.xlarge)'},
])

# m5.xlarge 1
# m5.xlarge 1
# m5.xlarge 2
d = pd.DataFrame([
    {'size': 1, 'time': 5.9, 'alg': 'hadoop', 'cores': '16 vcores (m5.xlarge)'},
    {'size': 5, 'time': 23, 'alg': 'hadoop', 'cores': '16 vcores (m5.xlarge)'},
    {'size': 10, 'time': 46, 'alg': 'hadoop', 'cores': '16 vcores (m5.xlarge)'},
])

custom = pd.DataFrame([
    {'size': 1, 'time': 0.32404, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 1, 'time': 0.3255, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 1, 'time': 0.32289, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 1, 'time': 0.4573, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 1, 'time': 1.052, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 1, 'time': 0.5511, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 1, 'time': 0.5587, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 5, 'time': 2.3996, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 5, 'time': 2.39456, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 5, 'time': 3.55877, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 5, 'time': 4.0321, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 10, 'time': 5.9326, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 10, 'time': 5.5393, 'alg': 'custom', 'cores': 'sequential (m5.large)'},
    {'size': 10, 'time': 8.6133, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
    {'size': 10, 'time': 7.57464, 'alg': 'custom', 'cores': 'sequential (m4.large)'},
])
df = pd.concat([custom, b, c, d, a])

sizes = {
    1: 0.32,
    5: 2.39,
    10: 5.62
}


def cost():
    df['cost'] = [prices[core] * time / 60 for core, time in df[['cores', 'time']].values]
    # df['cost'] = prices[df['cores']] * (df['time'] / 60)
    result = df.groupby(['size', 'cores']).mean()
    # s = result.style.highlight_min(color='red', axis=1).highlight_max(color='lightgreen', axis=1)
    # s = result.style.highlight_max(
    #     props='cellcolor:{yellow}; color:{red};'
    #           'textit:--rwrap; textbf:--rwrap;'
    # )
    print(result.to_latex())


def plot():
    sns.set_theme(style="darkgrid")

    ax = sns.pointplot(x="size", y="time", data=df,
                       hue='cores', legend='full', ci='sd')
    # ax.set(yscale="log")
    ax.legend(title='Cores configuration', loc='upper left')
    ax.set(ylabel='Duration [min]')
    ax.set_title(f'Execution duration based on a problem size')
    ax.set(xlabel='Size of problem [GB]')

    plt.show()

    df['speed up'] = [sizes[size] / time for size, time in df[['size', 'time']].values]
    # df['speed up'] = df.apply(lambda row: sizes[row['size']] / row['time'])
        # 0.52 / df['size' == 1]['time']
    ax = sns.pointplot(x="cores", y="speed up", data=df,
                       hue='size', legend='full', ci='sd')
    # ax.set(yscale="log")
    ax.legend(title='Size of a problem [GB]', loc='upper right')
    ax.set(ylabel='Speed up')
    ax.set_title(f'Speed up based on cores')
    ax.set(xlabel='Number of cores')

    plt.show()

    ax = sns.pointplot(x="cores", y="speed up", data=df,
                       hue='size', legend='full', ci='sd')
    ax.set(yscale="log")
    ax.legend(title='Size of a problem [GB]', loc='upper right')
    ax.set(ylabel='Speed up')
    ax.set_title(f'Speed up based on cores')
    ax.set(xlabel='Number of cores')

    plt.show()


if __name__ == '__main__':
    cost()
    # plot()
