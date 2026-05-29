# AI Smart Nesting Optimizer

## Vue d'ensemble

Le module **AI Smart Nesting Optimizer** est un système d'optimisation intelligent pour l'arrangement automatique de pièces en bois sur des panneaux, minimisant les déchets et maximisant l'efficacité de découpe pour les applications de menuiserie industrielle.

## Fonctionnalités principales

### 🧠 Algorithmes d'optimisation
- **Rectangle Packing Algorithm** : Placement optimal des pièces rectangulaires
- **Rotation automatique** : Essai des orientations 90° pour améliorer l'optimisation
- **Multi-panneaux** : Distribution intelligente sur plusieurs panneaux si nécessaire

### 🎨 Visualisation interactive
- **QGraphicsView** : Interface graphique pour la visualisation des panneaux et pièces
- **Couleurs uniques** : Chaque pièce a une couleur distincte pour l'identification
- **Info-bulles** : Détails des dimensions au survol
- **Zoom/Pan** : Navigation fluide dans les layouts complexes

### 📊 Analyses avancées
- **Tableau de bord d'efficacité** : Métriques en temps réel
- **Score IA** : Évaluation intelligente de la qualité d'optimisation
- **Pourcentage de déchets** : Calcul précis des pertes matérielles

### ⚙️ Contrôles utilisateur
- **Modes d'optimisation** :
  - Rapide (algorithme glouton)
  - Optimisé (équilibré)
  - IA avancée (essais multiples)
- **Tolérance de coupe** : Ajustement de l'espacement entre pièces
- **Mode simulation** : Animation pas-à-pas du placement

### 🤖 Fonctionnalités innovantes
- **Suggestions IA** : Conseils en temps réel pour améliorer les layouts
- **Drag & Adjust** : Réarrangement manuel des pièces
- **Export** : PDF et PNG des layouts optimisés

## Architecture technique

### Classes principales

#### NestingEngine
```cpp
class NestingEngine : public QObject
```
Moteur principal d'optimisation implémentant les algorithmes de placement.

**Méthodes clés :**
- `optimizeLayout()` : Fonction principale d'optimisation
- `generateOptimizationSuggestions()` : Analyse IA des résultats
- `calculateAIScore()` : Calcul du score d'optimisation

#### PieceRect
```cpp
class PieceRect
```
Représentation d'une pièce à placer avec ses dimensions et propriétés.

#### Panel
```cpp
class Panel
```
Représentation d'un panneau avec dimensions et propriétés matérielles.

#### NestingWidget
```cpp
class NestingWidget : public QWidget
```
Interface utilisateur principale avec visualisation QGraphicsView.

### Algorithmes implémentés

1. **Bottom-Left Fill** : Placement par le coin inférieur gauche
2. **Free Rectangle Management** : Gestion des espaces libres
3. **Look-ahead Optimization** : Anticipation des placements futurs

## Intégration

### Chargement des données
```cpp
// Chargement des pièces publiées
void loadPublishedPiecesToNesting();

// Chargement des panneaux disponibles
nestingWidget->loadPanels(nestingPanels);
```

### Déclenchement de l'optimisation
```cpp
// Démarrage de l'optimisation
nestingWidget->startOptimization();
```

## Métriques d'efficacité

### Score IA
Le score IA est calculé selon la formule :
```
Score = Efficacité × (1 - Pénalité_panneaux_supp)
```

Où :
- **Efficacité** = 100% - Pourcentage_déchets
- **Pénalité_panneaux_supp** = 0.1 × (Nombre_panneaux - 1)

### Niveaux de score
- **90-100%** : Excellent
- **80-89%** : Très bon
- **70-79%** : Bon
- **60-69%** : Moyen
- **< 60%** : À améliorer

## Utilisation

1. **Accès** : Onglet "🧠 AI Nesting Optimizer" dans le module Pièces
2. **Chargement** : Les pièces publiées sont automatiquement chargées
3. **Configuration** : Sélection du mode et tolérance de coupe
4. **Optimisation** : Clic sur "Optimiser découpe"
5. **Visualisation** : Observation du résultat avec métriques
6. **Export** : Sauvegarde du layout en PDF/PNG

## Technologies utilisées

- **Qt Widgets** : Interface utilisateur
- **QGraphicsView/QGraphicsScene** : Visualisation 2D
- **Algorithmes de bin packing** : Optimisation mathématique
- **C++17** : Langage de programmation

## Perspectives d'évolution

- **Machine Learning** : Apprentissage des patterns optimaux
- **Multi-matériaux** : Gestion de différents types de bois
- **3D Visualization** : Prévisualisation en trois dimensions
- **Cloud Integration** : Optimisation distribuée
- **IoT Connectivity** : Connexion directe aux machines CNC

---

*Module développé pour SmartCarpentry - Système de gestion de menuiserie intelligente*</content>
<parameter name="filePath">c:\Users\SETUP\OneDrive\Bureau\New folder (2)\Nouveau dossier\Nouveau dossier\AI_NESTING_OPTIMIZER_README.md