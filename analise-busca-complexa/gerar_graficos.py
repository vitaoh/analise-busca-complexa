"""
Gerador de Gráficos para TP1 - Algoritmos de Pesquisa
Lê os resultados do arquivo CSV gerado pelo programa em C
e gera gráficos comparativos automáticos
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from pathlib import Path
import os
import sys

# Configuração do estilo dos gráficos
plt.style.use('seaborn-v0_8-darkgrid')
sns.set_palette("husl")
plt.rcParams['figure.figsize'] = (12, 8)
plt.rcParams['font.size'] = 12

class GeradorGraficos:
    def __init__(self, arquivo_csv='output/resultados.csv', pasta_saida='graficos'):
        self.arquivo_csv = Path(arquivo_csv)
        self.diretorio_saida = Path(pasta_saida)
        self.diretorio_saida.mkdir(parents=True, exist_ok=True)
        self.df = None
        self.verificar_arquivo()

    def verificar_arquivo(self):
        """Verifica se o arquivo CSV existe"""
        if not os.path.exists(self.arquivo_csv):
            print(f"Erro: Arquivo '{self.arquivo_csv}' não encontrado.")
            print("Execute primeiro o programa em C para gerar o CSV.")
            sys.exit(1)

    def carregar_dados(self):
        """Carrega o arquivo CSV com os resultados"""
        try:
            self.df = pd.read_csv(self.arquivo_csv)
            return True
        except Exception as e:
            print(f"Erro ao carregar CSV: {e}")
            return False

    def grafico_tempo_execucao(self):
        """Gráfico comparando tempos de execução"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

        # Gráfico 1: Tempos absolutos
        x = range(len(self.df))
        width = 0.35

        ax1.bar([i - width/2 for i in x], self.df['Tempo_Sequencial_ms'], 
                width, label='Busca Sequencial', alpha=0.8, color='#e74c3c')
        ax1.bar([i + width/2 for i in x], self.df['Tempo_Binaria_ms'], 
                width, label='Busca Binária', alpha=0.8, color='#3498db')

        ax1.set_xlabel('Tamanho do Vetor')
        ax1.set_ylabel('Tempo de Execução (ms)')
        ax1.set_title('Comparação de Tempo de Execução')
        ax1.set_xticks(x)
        ax1.set_xticklabels([f'{int(t/1000)}K' for t in self.df['Tamanho']])
        ax1.legend()
        ax1.grid(True, alpha=0.3)

        # Gráfico 2: Escala logarítmica para melhor visualização
        ax2.semilogy(self.df['Tamanho'], self.df['Tempo_Sequencial_ms'], 
                     'o-', label='Busca Sequencial', linewidth=2, markersize=8, color='#e74c3c')
        ax2.semilogy(self.df['Tamanho'], self.df['Tempo_Binaria_ms'], 
                     's-', label='Busca Binária', linewidth=2, markersize=8, color='#3498db')

        ax2.set_xlabel('Tamanho do Vetor (N)')
        ax2.set_ylabel('Tempo de Execução (ms) - Escala Log')
        ax2.set_title('Tempo de Execução - Escala Logarítmica')
        ax2.legend()
        ax2.grid(True, alpha=0.3)

        plt.tight_layout()
        plt.savefig(self.diretorio_saida / 'grafico_tempo_execucao.png', dpi=300, bbox_inches='tight')

    def grafico_comparacoes(self):
        """Gráfico comparando número de comparações"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

        # Gráfico 1: Comparações absolutas
        x = range(len(self.df))
        width = 0.35

        ax1.bar([i - width/2 for i in x], self.df['Comp_Sequencial'], 
                width, label='Busca Sequencial', alpha=0.8, color='#f39c12')
        ax1.bar([i + width/2 for i in x], self.df['Comp_Binaria'], 
                width, label='Busca Binária', alpha=0.8, color='#27ae60')

        ax1.set_xlabel('Tamanho do Vetor')
        ax1.set_ylabel('Número Médio de Comparações')
        ax1.set_title('Comparação do Número de Comparações')
        ax1.set_xticks(x)
        ax1.set_xticklabels([f'{int(t/1000)}K' for t in self.df['Tamanho']])
        ax1.legend()
        ax1.grid(True, alpha=0.3)

        # Gráfico 2: Escala logarítmica
        ax2.semilogy(self.df['Tamanho'], self.df['Comp_Sequencial'], 
                     'o-', label='Busca Sequencial', linewidth=2, markersize=8, color='#f39c12')
        ax2.semilogy(self.df['Tamanho'], self.df['Comp_Binaria'], 
                     's-', label='Busca Binária', linewidth=2, markersize=8, color='#27ae60')

        ax2.set_xlabel('Tamanho do Vetor (N)')
        ax2.set_ylabel('Comparações - Escala Log')
        ax2.set_title('Número de Comparações - Escala Logarítmica')
        ax2.legend()
        ax2.grid(True, alpha=0.3)

        plt.tight_layout()
        plt.savefig(self.diretorio_saida / 'grafico_comparacoes.png', dpi=300, bbox_inches='tight')

    def grafico_complexidade_teorica(self):
        """Gráfico comparando resultados práticos com complexidade teórica"""
        # Gerar pontos teóricos
        n_teorico = np.linspace(10000, 100000, 100)
        linear_teorico = n_teorico / 2  # O(n) caso médio
        log_teorico = np.log2(n_teorico)  # O(log n)

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

        # Gráfico 1: Comparações práticas vs teóricas
        ax1.plot(n_teorico, linear_teorico, '--', alpha=0.7, color='#e74c3c', 
                label='Teórico O(n)', linewidth=2)
        ax1.plot(n_teorico, log_teorico * 100, '--', alpha=0.7, color='#3498db', 
                label='Teórico O(log n) × 100', linewidth=2)

        ax1.scatter(self.df['Tamanho'], self.df['Comp_Sequencial'], 
                   s=100, color='#c0392b', label='Sequencial Real', zorder=5)
        ax1.scatter(self.df['Tamanho'], self.df['Comp_Binaria'], 
                   s=100, color='#2980b9', label='Binária Real', zorder=5)

        ax1.set_xlabel('Tamanho do Vetor (N)')
        ax1.set_ylabel('Número de Comparações')
        ax1.set_title('Resultados Práticos vs Complexidade Teórica')
        ax1.legend()
        ax1.grid(True, alpha=0.3)

        # Gráfico 2: Razão de melhoria
        ax2.bar(range(len(self.df)), self.df['Reducao_Comparacoes'], 
                alpha=0.8, color='#9b59b6')
        ax2.set_xlabel('Tamanho do Vetor')
        ax2.set_ylabel('Fator de Redução (vezes)')
        ax2.set_title('Quantas Vezes a Busca Binária é Melhor')
        ax2.set_xticks(range(len(self.df)))
        ax2.set_xticklabels([f'{int(t/1000)}K' for t in self.df['Tamanho']])
        ax2.grid(True, alpha=0.3)

        # Adicionar valores nas barras
        for i, v in enumerate(self.df['Reducao_Comparacoes']):
            ax2.text(i, v + max(self.df['Reducao_Comparacoes']) * 0.01, 
                    f'{v:.1f}x', ha='center', va='bottom', fontweight='bold')

        plt.tight_layout()
        plt.savefig(self.diretorio_saida / 'grafico_complexidade_teorica.png', dpi=300, bbox_inches='tight')

    def grafico_speedup(self):
        """
        Gráfico mostrando o 'speedup' da busca binária.
        Speedup mede **quanto mais rápido** um algoritmo é em relação a outro.
        É calculado como:

            Speedup = Tempo_Sequencial / Tempo_Binaria

        Exemplo: se a busca binária leva 5 ms e a sequencial 50 ms,
        então Speedup = 50 / 5 = 10 → a busca binária é 10x mais rápida.
        """
        fig, ax = plt.subplots(figsize=(10, 6))

        bars = ax.bar(range(len(self.df)), self.df['Speedup_Tempo'], 
                     alpha=0.8, color=['#e67e22', '#d35400', '#a0522d'])

        ax.set_xlabel('Tamanho do Vetor')
        ax.set_ylabel('Speedup (vezes mais rápida)')
        ax.set_title('Speedup da Busca Binária vs Sequencial')
        ax.set_xticks(range(len(self.df)))
        ax.set_xticklabels([f'{int(t/1000)}K' for t in self.df['Tamanho']])
        ax.grid(True, alpha=0.3)

        # Adicionar valores nas barras
        for i, (bar, v) in enumerate(zip(bars, self.df['Speedup_Tempo'])):
            ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.1, 
                   f'{v:.1f}x', ha='center', va='bottom', fontweight='bold')

        plt.tight_layout()
        plt.savefig(self.diretorio_saida / 'grafico_speedup.png', dpi=300, bbox_inches='tight')

    def tabela_resultados(self):
        """Cria uma tabela formatada dos resultados"""
        fig, ax = plt.subplots(figsize=(14, 6))
        ax.axis('tight')
        ax.axis('off')

        # Preparar dados da tabela
        dados_tabela = [
            ['Tamanho (N)', 'Tempo Seq. (ms)', 'Tempo Bin. (ms)', 
             'Comp. Seq.', 'Comp. Bin.', 'Speedup', 'Redução Comp.']
        ]

        for _, row in self.df.iterrows():
            dados_tabela.append([
                f"{int(row['Tamanho']):,}",
                f"{row['Tempo_Sequencial_ms']:.3f}",
                f"{row['Tempo_Binaria_ms']:.3f}",
                f"{row['Comp_Sequencial']:.1f}",
                f"{row['Comp_Binaria']:.1f}",
                f"{row['Speedup_Tempo']:.1f}x",
                f"{row['Reducao_Comparacoes']:.1f}x"
            ])

        table = ax.table(cellText=dados_tabela[1:], colLabels=dados_tabela[0],
                        cellLoc='center', loc='center')
        table.auto_set_font_size(False)
        table.set_fontsize(10)
        table.scale(1.2, 1.5)

        # Formatação da tabela
        for i in range(len(dados_tabela[0])):
            table[(0, i)].set_facecolor('#34495e')
            table[(0, i)].set_text_props(weight='bold', color='white')

        plt.title('Resultados Completos do TP1 - Algoritmos de Pesquisa', 
                 fontsize=16, fontweight='bold', pad=20)
        plt.savefig(self.diretorio_saida / 'tabela_resultados.png', dpi=300, bbox_inches='tight')

    def gerar_todos_graficos(self):
        """Gera todos os gráficos"""
        if not self.carregar_dados():
            return False

        try:
            self.grafico_tempo_execucao()
            self.grafico_comparacoes()
            self.grafico_complexidade_teorica()
            self.grafico_speedup()
            self.tabela_resultados()

            return True

        except Exception as e:
            return False

def main():
    print("🚀 GERADOR DE GRÁFICOS - TP1 ALGORITMOS DE PESQUISA")

    gerador = GeradorGraficos()

    if gerador.gerar_todos_graficos():

        print("\n Concluído com sucesso:")

    else:
        print("\nFalha ao gerar gráficos!")

if __name__ == "__main__":
    main()