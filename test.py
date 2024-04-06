import math

def euclidean_distance(x1, y1, z1, x2, y2, z2):
    return math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2 + (z2 - z1) ** 2)

# Coordonnées GPS des deux points (longitude, latitude)
lon1, lat1 = 3.8696300012, 43.6084384564  # Exemple: San Francisco
lon2, lat2 = 3.8589325065, 43.5726881862  # Exemple: Los Angeles

# Rayon de la Terre en mètres
R = 6371000  

# Conversion des coordonnées en coordonnées cartésiennes
x1 = R * math.cos(math.radians(lat1)) * math.cos(math.radians(lon1))
y1 = R * math.cos(math.radians(lat1)) * math.sin(math.radians(lon1))
z1 = R * math.sin(math.radians(lat1))

x2 = R * math.cos(math.radians(lat2)) * math.cos(math.radians(lon2))
y2 = R * math.cos(math.radians(lat2)) * math.sin(math.radians(lon2))
z2 = R * math.sin(math.radians(lat2))

# Calcul de la distance euclidienne
distance = euclidean_distance(x1, y1, z1, x2, y2, z2)
print("Distance entre les deux points:", distance, "mètres")
#------------------------------------------------------------------------------
lat1, lon1 = 3.8696300012, 43.6084384564  # Exemple: San Francisco
lat2, lon2 = 3.8589325065, 43.5726881862  # Exemple: Los Angeles

# Rayon de la Terre en mètres
R = 6371000  

# Conversion des coordonnées en coordonnées cartésiennes
x1 = R * math.cos(math.radians(lat1)) * math.cos(math.radians(lon1))
y1 = R * math.cos(math.radians(lat1)) * math.sin(math.radians(lon1))
z1 = R * math.sin(math.radians(lat1))

x2 = R * math.cos(math.radians(lat2)) * math.cos(math.radians(lon2))
y2 = R * math.cos(math.radians(lat2)) * math.sin(math.radians(lon2))
z2 = R * math.sin(math.radians(lat2))

# Calcul de la distance euclidienne
distance = euclidean_distance(x1, y1, z1, x2, y2, z2)
print("Distance entre les deux points:", distance, "mètres")
#------------------------------------------------------------------------------
lon1, lat1 = 3.8696300012, 43.6084384564  # Exemple: San Francisco
lon2, lat2 = 3.8589325065, 43.5726881862  # Exemple: Los Angeles

# Rayon de la Terre en mètres
R = 6371000  

# Conversion des coordonnées en coordonnées cartésiennes
x1 = R * math.sin(math.radians(lat1)) * math.cos(math.radians(lon1))
y1 = R * math.sin(math.radians(lat1)) * math.sin(math.radians(lon1))
z1 = R * math.sin(math.radians(lat1))

x1 = R * math.sin(math.radians(lat2)) * math.cos(math.radians(lon2))
y1 = R * math.sin(math.radians(lat2)) * math.sin(math.radians(lon2))
z1 = R * math.sin(math.radians(lat2))

# Calcul de la distance euclidienne
distance = euclidean_distance(x1, y1, z1, x2, y2, z2)
print("Distance entre les deux points:", distance, "mètres")