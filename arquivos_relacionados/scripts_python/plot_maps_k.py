import pandas as pd
import folium
import os
import glob

# 1. Configurações de Caminhos
input_folder = 'arquivos_relacionados/saidas_cpp'
output_folder = 'arquivos_relacionados/plots'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Busca todos os arquivos CSV gerados pelo C++
files = glob.glob(os.path.join(input_folder, "grafo_visual_k*.csv"))

if not files:
    print(f"ERRO: Nenhum arquivo CSV encontrado em '{input_folder}'.")
    print("Certifique-se de ter rodado 'make run' primeiro.")
    exit()

print(f"Encontrados {len(files)} arquivos para processar.")

# Coordenadas centrais de Nova York para iniciar o mapa
NY_COORDS = [40.7128, -74.0060]

# 2. Função para processar cada arquivo
for file_path in files:
    filename = os.path.basename(file_path)
    
    # Tenta extrair o número K
    try:
        k_val = filename.replace('grafo_visual_k', '').replace('.csv', '')
    except:
        k_val = "?"

    print(f"Gerando mapa para {filename} (K={k_val})...")

    try:
        df = pd.read_csv(file_path)
    except Exception as e:
        print(f"Erro ao ler {filename}: {e}")
        continue

    # Cria o Mapa Base
    m = folium.Map(location=NY_COORDS, zoom_start=11, tiles='CartoDB positron')

    # Título flutuante
    title_html = f'''
             <h3 align="center" style="font-size:16px"><b>Cenário K={k_val}</b></h3>
             '''
    m.get_root().html.add_child(folium.Element(title_html))

    df_mst = df[df['type'] == 'MST']
    df_new = df[df['type'] == 'New_Connection']

    mst_group = folium.FeatureGroup(name='MST (Base)')
    for _, row in df_mst.iterrows():
        points = [(row['lat1'], row['lon1']), (row['lat2'], row['lon2'])]
        folium.PolyLine(points, color='blue', weight=2.0, opacity=0.8).add_to(mst_group)
    mst_group.add_to(m)

    new_conn_group = folium.FeatureGroup(name=f'Novas Conexões (K={k_val})')
    for _, row in df_new.iterrows():
        points = [(row['lat1'], row['lon1']), (row['lat2'], row['lon2'])]
        folium.PolyLine(points, color='red', weight=3.0, opacity=1.0).add_to(new_conn_group)
    new_conn_group.add_to(m)

    # Adiciona controle de camadas
    folium.LayerControl().add_to(m)

    # Salva o arquivo HTML
    output_filename = f"mapa_visual_k{k_val}.html"
    output_path = os.path.join(output_folder, output_filename)
    m.save(output_path)
    print(f" -> Mapa salvo em: {output_path}")

print("\nProcesso concluído! Abra os arquivos .html na pasta 'plots'.")