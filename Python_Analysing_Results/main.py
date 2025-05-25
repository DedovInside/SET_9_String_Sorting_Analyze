import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных
data = pd.read_csv("sorts_results.csv")

# Подготовка данных для графиков
sizes = sorted(data['size'].unique())
types = ['random', 'reverse', 'almost', 'prefix']
algorithms = ['quick_sort', 'merge_sort', 'string_quicksort', 'string_mergesort', 'msd_radix_sort', 'msd_radix_sort_switch']

# Построение графиков времени выполнения
for t in types:
    plt.figure(figsize=(8, 8))  # Формат 1:1
    for algo in algorithms:
        subset = data[(data['type'] == t) & (data['algorithm'] == algo)]
        plt.plot(subset['size'], subset['time_ms'], label=algo, marker='o')
    plt.title(f'Время выполнения для {t} массива')
    plt.xlabel('Размер массива')
    plt.ylabel('Время (мс)')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'time_{t}.png', bbox_inches='tight')

# Построение графиков количества сравнений
for t in types:
    plt.figure(figsize=(8, 8))  # Формат 1:1
    for algo in algorithms:
        subset = data[(data['type'] == t) & (data['algorithm'] == algo)]
        plt.plot(subset['size'], subset['comparisons'], label=algo, marker='o')
    plt.title(f'Количество посимвольных сравнений для {t} массива')
    plt.xlabel('Размер массива')
    plt.ylabel('Сравнения')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'comparisons_{t}.png', bbox_inches='tight')