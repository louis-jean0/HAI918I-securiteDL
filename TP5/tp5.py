import tensorflow as tf
from tensorflow.keras import datasets, layers, models
import matplotlib.pyplot as plt

# Charger le jeu de données MNIST
(train_images, train_labels), (test_images, test_labels) = datasets.mnist.load_data()

# Redimensionner les images pour avoir une seule dimension de profondeur (1 canal pour les images en niveaux de gris)
train_images = train_images.reshape((60000, 28, 28, 1))
test_images = test_images.reshape((10000, 28, 28, 1))

# Normaliser les valeurs des pixels entre 0 et 1
train_images, test_images = train_images / 255.0, test_images / 255.0

# Construction du modèle CNN
model = models.Sequential()

# Ajout des couches
model.add(layers.Conv2D(32, (3, 3), activation='relu', input_shape=(28, 28, 1)))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(64, (3, 3), activation='relu'))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(64, (3, 3), activation='relu'))

# Ajout des couches de classification (fully connected)
model.add(layers.Flatten())
model.add(layers.Dense(64, activation='relu'))
model.add(layers.Dense(10, activation='softmax'))

# Compilation du modèle
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

# Entraînement du modèle
history = model.fit(train_images, train_labels, epochs=5, 
                    validation_data=(test_images, test_labels))

# Évaluation du modèle
test_loss, test_acc = model.evaluate(test_images, test_labels, verbose=2)
print(f'\nPrécision test: {test_acc}')

# Affichage des courbes d'apprentissage
plt.plot(history.history['accuracy'], label='Précision d\'entraînement')
plt.plot(history.history['val_accuracy'], label='Précision de test')
plt.xlabel('Epoque')
plt.ylabel('Précision')
plt.legend(loc='lower right')
plt.show()