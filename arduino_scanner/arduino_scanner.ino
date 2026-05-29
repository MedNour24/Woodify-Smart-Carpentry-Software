// ---------------------------------------------------------
// Smart Carpentry - Module de détection de plaque de bois
// ---------------------------------------------------------
// Ce script permet d'utiliser un capteur Infrarouge pour
// détecter la présence d'une plaque de bois, puis d'utiliser
// un capteur de distance (HC-SR04) pour estimer ses dimensions.
// Les données sont ensuite transmises via le port Série (USB)
// à l'application Qt.
// ---------------------------------------------------------

// --- Broches des capteurs ---
const int pinIR = 2;        // Capteur Infrarouge (détection présence)
const int pinTrig = 8;      // HC-SR04 Trigger
const int pinEcho = 9;      // HC-SR04 Echo

// Variables d'état
bool plaquePrecedenteDetectee = false;

void setup() {
  // Initialisation de la communication série à 9600 bauds
  Serial.begin(9600);

  // Configuration des broches
  pinMode(pinIR, INPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  // Valeur par défaut
  digitalWrite(pinTrig, LOW);
}

void loop() {
  // Le capteur IR renvoie LOW s'il détecte un objet (dépend du modèle, ici actif à l'état BAS)
  int etatIR = digitalRead(pinIR);
  bool plaqueDetectee = (etatIR == LOW);

  // Détection du front montant (nouvelle plaque vient d'arriver)
  if (plaqueDetectee && !plaquePrecedenteDetectee) {
    // Une courte pause pour simuler ou laisser l'utilisateur bien placer la plaque
    delay(1000);

    // 1. Démarrer le processus de scan avec le capteur de distance
    // Pour cet exemple de "demi-scénario", nous simulons le fait que 
    // l'épaisseur est fixe (2 cm) et la longueur est extrapolée.
    // Dans la vraie vie, l'HC-SR04 sur un rail mobile prendrait plusieurs points.
    
    // Mesure simple de distance (qui pourrait correspondre à la largeur scannée)
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig, LOW);

    long duree = pulseIn(pinEcho, HIGH, 30000); // Timeout court
    
    // La distance en cm ((durée * 0.034) / 2)
    float distanceCm = (duree * 0.0343) / 2.0;
    
    // Extrapolation des dimensions de la plaque :
    // Par exemple, si on détecte une surface à X centimètres, on en déduit la taille.
    // Ici, pour coller au scénario attendu par l'application : <Longueur,Largeur,Epaisseur>
    
    float longueur = 150.0;
    float largeur = 80.0;
    float epaisseur = 2.0;

    // Si le capteur a bien mesuré *quelque chose*, on peut associer la largeur à la distance mesurée.
    // Exemple : la plaque cache la distance maximale (100cm), donc largeur = 100 - distanceCm;
    if (distanceCm > 0 && distanceCm < 100) {
       largeur = 100.0 - distanceCm;
    }

    // Format de sortie attendu par Qt:
    // DATA;longueur;largeur;epaisseur
    Serial.print("DATA;");
    Serial.print(longueur, 1);
    Serial.print(";");
    Serial.print(largeur, 1);
    Serial.print(";");
    Serial.println(epaisseur, 1);
    
    // On évite les envois multiples pour la même plaque
    delay(2000); 
  }

  plaquePrecedenteDetectee = plaqueDetectee;

  // Courte pause de stabilité
  delay(100);
}
