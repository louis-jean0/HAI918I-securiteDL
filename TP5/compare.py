import tensorflow as tf
from tensorflow.keras import datasets, layers, models
import matplotlib.pyplot as plt

# Charger et prétraiter le jeu de données MNIST
def load_and_prepare_data():
    (train_images, train_labels), (test_images, test_labels) = datasets.mnist.load_data()
    train_images = train_images.reshape((60000, 28, 28, 1)) / 255.0
    test_images = test_images.reshape((10000, 28, 28, 1)) / 255.0
    return (train_images, train_labels), (test_images, test_labels)

# Construire un modèle CNN
def create_cnn_model(optimizer='adam', filters_1=32, filters_2=64, dense_units=64):
    model = models.Sequential()
    model.add(layers.Conv2D(filters_1, (3, 3), activation='relu', input_shape=(28, 28, 1)))
    model.add(layers.MaxPooling2D((2, 2)))
    model.add(layers.Conv2D(filters_2, (3, 3), activation='relu'))
    model.add(layers.MaxPooling2D((2, 2)))
    model.add(layers.Conv2D(filters_2, (3, 3), activation='relu'))
    model.add(layers.Flatten())
    model.add(layers.Dense(dense_units, activation='relu'))
    model.add(layers.Dense(10, activation='softmax'))

    # Compiler le modèle avec l'optimiseur choisi
    model.compile(optimizer=optimizer,
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
    return model

# Entraîner et évaluer le modèle
def train_and_evaluate_model(model, train_images, train_labels, test_images, test_labels, epochs=5):
    history = model.fit(train_images, train_labels, epochs=epochs, 
                        validation_data=(test_images, test_labels), verbose=0)
    test_loss, test_acc = model.evaluate(test_images, test_labels, verbose=0)
    return history, test_acc

# Tracer les courbes d'apprentissage
def plot_learning_curves(history, title='Précision du modèle'):
    plt.plot(history.history['accuracy'], label='Précision d\'entraînement')
    plt.plot(history.history['val_accuracy'], label='Précision de test')
    plt.xlabel('Epoque')
    plt.ylabel('Précision')
    plt.title(title)
    plt.legend(loc='lower right')
    plt.show()

# Comparer différents paramètres
def compare_models():
    # Charger les données
    (train_images, train_labels), (test_images, test_labels) = load_and_prepare_data()

    # Liste des configurations à tester
    optimizers = ['adam', 'sgd', 'rmsprop']  # Différents optimisateurs
    epochs_list = [5, 10]  # Différents nombres d'époques

    # Stocker les résultats
    results = {}

    for optimizer in optimizers:
        for epochs in epochs_list:
            # Créer et entraîner le modèle
            model = create_cnn_model(optimizer=optimizer)
            history, test_acc = train_and_evaluate_model(model, train_images, train_labels, test_images, test_labels, epochs=epochs)

            # Sauvegarder et afficher les résultats
            config = f'Optimiseur: {optimizer}, Epoques: {epochs}'
            results[config] = test_acc
            print(f'{config} - Précision de test: {test_acc:.4f}')

            # Tracer les courbes d'apprentissage
            plot_learning_curves(history, title=config)
    
    # Afficher tous les résultats
    print("\nSummary of Results:")
    for config, acc in results.items():
        print(f'{config}: {acc:.4f}')

# Lancer la comparaison
compare_models()