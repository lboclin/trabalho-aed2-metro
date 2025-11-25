import pandas as pd
import folium
import os

# --- Configuração ---
# Pode pegar qualquer arquivo, pois todos contêm todas as estações (mesmo que conectadas)
input_file = 'arquivos_relacionados/saidas_cpp/grafo_visual_k0.csv'
output_file = 'arquivos_relacionados/plots/mapa_estacoes_limpo.html'

# Garante que a pasta de saída existe
if not os.path.exists('arquivos_relacionados/plots'):
    os.makedirs('arquivos_relacionados/plots')

# 1. Carregar os dados
try:
    df = pd.read_csv(input_file)
except FileNotFoundError:
    print(f"Erro: O arquivo {input_file} não existe.")
    print("Rode 'make run' primeiro.")
    exit()

# 2. Extrair Estações Únicas
# O CSV tem arestas (Lat1, Lon1 -> Lat2, Lon2). Precisamos empilhar tudo numa lista só.
origins = df[['lat1', 'lon1']].rename(columns={'lat1': 'lat', 'lon1': 'lon'})
destinations = df[['lat2', 'lon2']].rename(columns={'lat2': 'lat', 'lon2': 'lon'})

# Concatena e remove duplicatas para ter apenas os pontos únicos
all_stations = pd.concat([origins, destinations]).drop_duplicates()

print(f"Plotando {len(all_stations)} estações no mapa...")

# 3. Criar o Mapa
# 'CartoDB positron' deixa o fundo limpo para destacar os pontos
m = folium.Map(location=[40.7128, -74.0060], zoom_start=11, tiles='CartoDB positron')

# Título
title_html = '''<h3 align="center" style="font-size:16px"><b>Distribuição das Estações (Sem Conexões)</b></h3>'''
m.get_root().html.add_child(folium.Element(title_html))

# 4. Adicionar os Pontos
for _, row in all_stations.iterrows():
    folium.CircleMarker(
        location=[row['lat'], row['lon']],
        radius=3,           # Tamanho da bolinha
        color='black',      # Cor da borda
        fill=True,
        fill_color='black', # Cor do preenchimento
        fill_opacity=1.0,
        weight=0            # Sem borda grossa
    ).add_to(m)

# 5. Salvar
m.save(output_file)
print(f"Mapa salvo em: {output_file}")