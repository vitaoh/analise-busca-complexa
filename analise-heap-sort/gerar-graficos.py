import matplotlib.pyplot as plt
import numpy as np
import csv

# Lê o CSV gerado pelo C
tamanhos = []
tempos = []
comparacoes = []
comp_teorico = []
razoes = []

with open('resultados-heap-sort.csv', 'r', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for row in reader:
        tamanhos.append(int(row['N']))
        tempos.append(float(row['Tempo_Medio_segundos']))
        comparacoes.append(int(row['Numero_Medio_Comparacoes']))
        comp_teorico.append(float(row['Complexidade_Teorica']))
        razoes.append(float(row['Razao_Pratico_Teorico']))

tamanhos = np.array(tamanhos)
tempos = np.array(tempos)
comparacoes = np.array(comparacoes)
comp_teorico = np.array(comp_teorico)
razoes = np.array(razoes)

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# Gráfico 1: Tempo de Execução e Comparações
color1 = 'tab:blue'
ax1.set_xlabel('Tamanho do Array (N)', fontsize=12, fontweight='bold')
ax1.set_ylabel('Tempo de Execução (segundos)', color=color1, fontsize=12, fontweight='bold')
ax1.plot(tamanhos, tempos, color=color1, marker='o', linewidth=2, markersize=8, label='Tempo de Execução')
ax1.tick_params(axis='y', labelcolor=color1)
ax1.grid(True, alpha=0.3)

ax1_twin = ax1.twinx()
color2 = 'tab:red'
ax1_twin.set_ylabel('Número de Comparações', color=color2, fontsize=12, fontweight='bold')
ax1_twin.plot(tamanhos, comparacoes, color=color2, marker='s', linewidth=2, markersize=8, label='Comparações', linestyle='--')
ax1_twin.tick_params(axis='y', labelcolor=color2)

ax1.set_title('Desempenho do Heap Sort:\nTempo e Comparações vs Tamanho do Array', fontsize=13, fontweight='bold', pad=15)
ax1.legend(loc='upper left', fontsize=10)
ax1_twin.legend(loc='upper right', fontsize=10)
ax1.set_xticks(tamanhos)
ax1.set_xticklabels([f'{n:,}' for n in tamanhos])

# Gráfico 2: Complexidade Teórica vs Prática
ax2.plot(tamanhos, comp_teorico, color='tab:blue', marker='o', linewidth=2.5, markersize=8, label='O(N log N) Teórico')
ax2.plot(tamanhos, comparacoes, color='tab:red', marker='s', linewidth=2.5, markersize=8, label='Comparações Reais', linestyle='--')

ax2.set_xlabel('Tamanho do Array (N)', fontsize=12, fontweight='bold')
ax2.set_ylabel('Número de Operações/Comparações', fontsize=12, fontweight='bold')
ax2.set_title('Complexidade Teórica vs Prática do Heap Sort', fontsize=13, fontweight='bold', pad=15)
ax2.grid(True, alpha=0.3)
ax2.legend(fontsize=11, loc='upper left')
ax2.set_xticks(tamanhos)
ax2.set_xticklabels([f'{n:,}' for n in tamanhos])
ax2.yaxis.set_major_formatter(plt.FuncFormatter(lambda x, p: f'{int(x):,}'))

plt.tight_layout()
plt.savefig('graficos-heap-sort.png', dpi=300, bbox_inches='tight')
print("Gráficos salvos em: graficos-heap-sort.png")
plt.show()