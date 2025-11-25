import pandas as pd
import folium
import os
import glob

# 1. Configurações de Caminhos
input_folder = 'arquivos_relacionados/saidas_cpp'
output_folder = 'arquivos_relacionados/plots'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Busca todos os arquivos CSV com o padrão "smart"
files = glob.glob(os.path.join(input_folder, "grafo_visual_smart_*.csv"))

if not files:
    print(f"ERRO: Nenhum arquivo CSV 'smart' encontrado em '{input_folder}'.")
    print("Certifique-se de ter rodado 'make run' com a análise smart ativada.")
    exit()

print(f"Encontrados {len(files)} arquivos Smart para processar.")

# Coordenadas centrais de Nova York
NY_COORDS = [40.7128, -74.0060]

# 2. Função para processar cada arquivo
for file_path in files:
    filename = os.path.basename(file_path)
    
    # Extrai o fator X do nome do arquivo (ex: grafo_visual_smart_1.500000.csv -> 1.5)
    try:
        # Remove o prefixo e a extensão
        val_str = filename.replace('grafo_visual_smart_', '').replace('.csv', '')
        # Tenta converter para float para garantir que é número, depois volta pra string limpa
        factor_val = float(val_str)
        # Formata para string curta se tiver muitos zeros (ex: 1.5 em vez de 1.500000)
        factor_label = f"{factor_val:.1f}"
    except:
        factor_label = "?"

    print(f"Gerando mapa para {filename} (Fator={factor_label})...")

    try:
        df = pd.read_csv(file_path)
    except Exception as e:
        print(f"Erro ao ler {filename}: {e}")
        continue

    # Cria o Mapa Base
    m = folium.Map(location=NY_COORDS, zoom_start=11, tiles='CartoDB positron')

    # Título flutuante
    title_html = f'''
             <h3 align="center" style="font-size:16px"><b>Algoritmo Smart: Fator {factor_label}x</b></h3>
             '''
    m.get_root().html.add_child(folium.Element(title_html))

    df_mst = df[df['type'] == 'MST']
    df_new = df[df['type'] == 'New_Connection']

    # --- Desenha a MST (AZUL) ---
    mst_group = folium.FeatureGroup(name='MST (Base)')
    for _, row in df_mst.iterrows():
        points = [(row['lat1'], row['lon1']), (row['lat2'], row['lon2'])]
        # Azul forte com boa visibilidade
        folium.PolyLine(points, color='blue', weight=2.0, opacity=0.8).add_to(mst_group)
    mst_group.add_to(m)

    # --- Desenha as Novas Conexões (VERMELHO) ---
    # Aqui mostramos onde o algoritmo inteligente decidiu criar atalhos
    new_conn_group = folium.FeatureGroup(name=f'Novas Conexões (Fator {factor_label})')
    for _, row in df_new.iterrows():
        points = [(row['lat1'], row['lon1']), (row['lat2'], row['lon2'])]
        # Vermelho sólido e mais grosso para destaque
        folium.PolyLine(points, color='red', weight=3.0, opacity=1.0).add_to(new_conn_group)
    new_conn_group.add_to(m)

    # Adiciona controle de camadas
    folium.LayerControl().add_to(m)

    # Salva o arquivo HTML com o nome solicitado: mapa_visual_x1.2.html
    output_filename = f"mapa_visual_x{factor_label}.html"
    output_path = os.path.join(output_folder, output_filename)
    m.save(output_path)
    print(f" -> Mapa salvo em: {output_path}")

print("\nProcesso concluído! Abra os arquivos .html na pasta 'plots'.")