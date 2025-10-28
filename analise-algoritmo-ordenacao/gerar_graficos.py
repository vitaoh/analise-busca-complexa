import pandas as pd
import matplotlib.pyplot as plt
import os

# 🔹 Criar pasta de saída automaticamente
pasta_imagens = "imagens"
os.makedirs(pasta_imagens, exist_ok=True)

plt.style.use('seaborn-v0_8-whitegrid')
plt.rcParams.update({'font.size': 12, 'figure.figsize': (10, 6)})

# 1️⃣ Ler arquivo CSV
df = pd.read_csv("resultados.csv")

# Exibir tabela no terminal
print("\n===== RESULTADOS =====\n")
print(df.to_string(index=False))

# Dicionário de cores fixas para cada algoritmo
cores = {
    "Bubble Sort": "#e74c3c",     # vermelho
    "Insertion Sort": "#2ecc71",  # verde
    "Selection Sort": "#9b59b6",  # roxo
    "Merge Sort": "#3498db"       # azul
}

# 2️⃣ Gráfico: Tempo de Execução
plt.figure()
for algoritmo in df["Algoritmo"].unique():
    dados = df[df["Algoritmo"] == algoritmo]
    plt.plot(dados["N"], dados["TempoSegundos"], marker="o", linewidth=2,
             label=algoritmo, color=cores.get(algoritmo, None))

plt.title("Tempo Médio de Execução (s)")
plt.xlabel("Tamanho do Vetor (N)")
plt.ylabel("Tempo (segundos)")
plt.legend(title="Algoritmos", loc="upper left")
plt.tight_layout()
plt.savefig(f"{pasta_imagens}/grafico_tempo_execucao.png", dpi=300, bbox_inches="tight", pad_inches=0.4)
plt.show()

# 3️⃣ Gráfico: Número de Comparações
plt.figure()
for algoritmo in df["Algoritmo"].unique():
    dados = df[df["Algoritmo"] == algoritmo]
    plt.plot(dados["N"], dados["Comparacoes"], marker="o", linewidth=2,
             label=algoritmo, color=cores.get(algoritmo, None))

plt.title("Número Médio de Comparações (escala logarítmica)")
plt.xlabel("Tamanho do Vetor (N)")
plt.ylabel("Comparações")
plt.yscale("log")
plt.legend(title="Algoritmos", loc="upper left")
plt.tight_layout()
plt.savefig(f"{pasta_imagens}/grafico_comparacoes.png", dpi=300, bbox_inches="tight", pad_inches=0.4)
plt.show()

# 4️⃣ Tabela estilo relatório (melhorada)
fig, ax = plt.subplots(figsize=(9, 3))
ax.axis('tight')
ax.axis('off')

tabela = []
for _, row in df.iterrows():
    tabela.append([
        row["Algoritmo"],
        f"{int(row['N']):,}".replace(",", "."),
        f"{row['TempoSegundos']:.6f}",
        f"{row['Comparacoes']:,}".replace(",", ".")
    ])

colunas = ["Algoritmo", "N", "Tempo de Execução Médio (segundos)", "Número Médio de Comparações"]
table = ax.table(cellText=tabela, colLabels=colunas, cellLoc='center', loc='center')

# Ajuste de formatação
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1.2, 1.4)

# Estilo do cabeçalho e linhas
for (i, j), cell in table.get_celld().items():
    if i == 0:
        cell.set_text_props(weight='bold', color='white')
        cell.set_facecolor('#34495e')
    else:
        cell.set_facecolor('#ecf0f1')

plt.title("Tabela de Resultados dos Algoritmos de Ordenação", pad=30, fontsize=14, fontweight='bold')

# Salvar imagem na pasta
plt.savefig(f"{pasta_imagens}/tabela_resultados.png", dpi=300, bbox_inches="tight", pad_inches=0.4)
plt.show()

print(f"\n✅ Imagens salvas na pasta: '{pasta_imagens}/'")