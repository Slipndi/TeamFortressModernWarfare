# 🎮 TFModerneWarfare – UE5 Project

Projet de shooter tactique multijoueur moderne en **Unreal Engine 5.3+**, avec une architecture modulaire orientée composant, des systèmes de réplication optimisés, et une base solide pour itérer en équipe rapidement.

---

## 📦 Structure Générale

| Élément | Description |
|--------|-------------|
| `TFC_PlayerBase` | Classe de personnage de base avec Health, Armor, Movement, WeaponSystem |
| `TFC_WeaponSlotManagerComponent` | Composant de gestion des armes par slot |
| `AWeaponBase`, `AWeapon_Hitscan` | Hiérarchie d’armes (base & spécialisation hitscan) |
| `TFC_MovementComponent` | Custom movement component pour marcher/sprinter/sliding |
| `TFC_ArmorComponent` | Gère les types d’armures et leur absorption de dégâts |
| `TFC_HealthComponent` | Gère les dégâts et la mort |
| `TFC_InputManagerComponent` | Composant pour centraliser tous les inputs |
| `TFC_GameMode`, `TFC_PlayerController` | Game loop & gestion joueur |
| `DataAssets` | Données d’armes, classes, configs input, etc. |
| `Widgets` | HUD et debug UI |

---

## 🔁 Fonctionnement & Imbrication

- **PlayerBase** : Le cœur de chaque joueur, compose tous les sous-systèmes.
- **WeaponSlotManagerComponent** : Spawne les armes selon un `WeaponDataAsset`, les attache et permet de tirer avec la current weapon.
- **InputManagerComponent** : Capte tous les inputs (tir, switch arme, sprint, etc.) et les relaye aux composants concernés.
- **Armor & Health** : Appliquent des dégâts avec absorption en fonction de l’`ArmorType`.
- **MovementComponent** : Gère état de mouvement (Walk, Sprint, Slide), vitesses dynamiques, et effet de transition.
- **Replication** : Le serveur spawne & gère la logique, les clients reçoivent les updates via `Multicast` ou `RepNotify`.

---

## 🧠 Classe & Méthodes (Sélection)

### `UTFC_WeaponSlotManagerComponent`

- `LoadWeaponSet(WeaponData)` : Charge les armes à partir d’un DataAsset.
- `SpawnAndAttachWeapon(Slot, Stats)` : Spawne et attache l’arme au personnage.
- `FireCurrentWeapon()` : Tire avec l’arme du slot courant.
- `Server_FireCurrentWeapon()` / `Multicast_PlayFireEffects()` : RPC réseau.

### `TFC_PlayerBase`

- `BeginPlay()` : Init des composants.
- `ApplyArmor()` : Récupère l’`ArmorData` et l’applique.
- `SetupPlayerInput()` : Branches inputs via l’InputManager.

---

## 📋 To-Do Ultra Complet

| Tâche | Priorité | Détails |
|------|----------|---------|
| ✅ WeaponSlotManager - Load depuis DataAsset | Important | Fait ✔ |
| ⌛ Refactor Weapon Attachment (no mesh fallback) | Modéré | Si mesh manquant, fallback sur transform brut |
| 🔧 Refactor de `AWeapon_Hitscan::Fire()` | Important | Modulariser le raycast & impact logic |
| ➕ Ajouter `AWeapon_Projectile` | Important | Armes avec projectiles physiques |
| 💥 Implémenter système de dégâts localisés | Modéré | Tête / Corps / Jambes |
| 🧠 Système de state joueur (équipe, vie, classe) | Important | Passer en `TFC_PlayerState` pour infos réseau |
| 🧃 UI d’armes dans le HUD | Modéré | Nom, munitions, slot actif |
| 🎨 Ajout de mesh & animations | Détail | Pour sockets, idle anim, etc. |
| 🧪 Weapon test UI dans le mode debug | Modéré | Swapper armes & tester les tirs |
| 🔄 Interpolation smooth des armes client | Important | Pour éviter les sauts d'attache |
| 📦 Créer `GameInstance` custom pour Data Bootstrap | Modéré | Charger tous les DA une fois au boot |
| 🚦 Améliorer gestion du `Authority` dans les logs | Détail | Nettoyer les logs & bien différencier client/server |
| 📂 Refacto Assets (naming conventions) | Modéré | BP_*, TFC_*, FX_, SFX_, etc. |
| 💼 Ajout de composants team-based | Important | `TFC_TeamComponent`, `TeamID`, gestion alliés/ennemis |
| 💣 Implémenter munitions + rechargement | Modéré | Logique + UI feedback |
| 📡 Optimiser réplication des composants | Important | Ne répliquer que ce qui est nécessaire |
| 🧱 Implémenter base de cover system | Détail | Pour gameplay futur |
| 🎯 Ajout d’un système de visée | Modéré | Zoom in/out avec FOV ou blend cam |
| 🌐 Online session & join via UI | Important | Utiliser `OnlineSubsystem` pour LAN/Online |
| 🔒 Ajout de permission sur les inputs client | Important | Empêcher tir client-side non validé |

---

## 📚 Références utiles

- **[Unreal Docs](https://docs.unrealengine.com/5.0/en-US/)** – Documentation officielle
- **Lyra Sample** – Référence pour composantisation & gameplay features
- **ShooterGame** – Exemples complets multi avec C++/BP

---

## 👨‍💻 Pour la suite

- Mettre en place **Git branch system** (`dev`, `feature/*`, `release`)
- Ajouter **Git LFS** si usage de fichiers lourds
- Créer une **doc interne Notion / Confluence** pour centraliser
- Préparer un système de **mod support/plugin** pour armes

---

## 🧠 Tips

- Prioriser la **clean architecture** : 1 système = 1 composant
- Faire une passe de **replication logic** → serveur au centre
- Ajouter un **profil debug mode** avec keybinds utilitaires
- Travailler avec des **DataAssets partout**, même pour UI

---
