import pandas as pd
import matplotlib.pyplot as plt

try:
    df = pd.read_csv('results.csv')
except FileNotFoundError:
    print("Ошибка: Файл results.csv не найден.")
    exit(1)

array_types = df['ArrayType'].unique()

algorithms = df['Algorithm'].unique()
colors = ['blue', 'orange', 'green', 'red', 'purple', 'brown']
markers = ['o', 's', '^', 'D', 'v', 'p']

print("Генерация графиков началась...")

for array_type in array_types:
    subset = df[df['ArrayType'] == array_type]
    
    fig, axes = plt.subplots(1, 2, figsize=(16, 6))
    fig.suptitle(f'Сравнение алгоритмов сортировки: {array_type} массивы', fontsize=16)
    
    for i, algo in enumerate(algorithms):
        algo_data = subset[subset['Algorithm'] == algo]
        axes[0].plot(algo_data['Size'], algo_data['TimeMs'], 
                     marker=markers[i % len(markers)], color=colors[i % len(colors)], 
                     label=algo, linewidth=2, markersize=6)
        
    axes[0].set_title('Время выполнения от размера массива')
    axes[0].set_xlabel('Размер массива (N)')
    axes[0].set_ylabel('Время (миллисекунды)')
    axes[0].legend()
    axes[0].grid(True, linestyle='--', alpha=0.7)
    
    for i, algo in enumerate(algorithms):
        algo_data = subset[subset['Algorithm'] == algo]
        axes[1].plot(algo_data['Size'], algo_data['Comparisons'], 
                     marker=markers[i % len(markers)], color=colors[i % len(colors)], 
                     label=algo, linewidth=2, markersize=6)
        
    axes[1].set_title('Количество сравнений от размера массива')
    axes[1].set_xlabel('Размер массива (N)')
    axes[1].set_ylabel('Количество посимвольных сравнений')
    axes[1].legend()
    axes[1].grid(True, linestyle='--', alpha=0.7)
    
    plt.tight_layout()
    filename = f'plot_{array_type}.png'
    plt.savefig(filename, dpi=300)
    plt.close()
    print(f"Сохранен график: {filename}")

print("Успех! Все графики успешно сгенерированы.")