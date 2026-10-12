# Propagation and Detection of Disinformation in an Information Network

A C application that models an information network as a **directed graph**, where each node is an article and each edge is a citation between two articles. It simulates how information (or fake news) spreads and detects suspicious articles.

## Features

- Load a network from a text file
- Add / delete articles and citations
- Display the network (adjacency lists)
- Find articles cited by, or citing, a given article
- Find original sources and isolated articles
- Find the most cited article
- Sort articles by publication date
- Find the first citing article and rebuild the propagation chain
- Simulate propagation with **BFS** (level by level)
- Analyze articles and compute a suspicion / reliability score
- List the most cited suspicious articles
- Bonus: simulate the deletion of an article and see its impact

## Project structure

| File | Purpose |
|------|---------|
| `main.c` | Interactive menu |
| `grapheReseau.c / .h` | Graph management (create, load, add, delete) |
| `adjList.c / .h` | Adjacency list operations |
| `file.c / .h` | Queue (FIFO) used by BFS |
| `ELTARTICLE.c / .h` | Article structure and date handling |
| `fakesdb.h` | Database of suspicious phrases and words |
| `reseau.txt` | Example network data |

## How it works

- **Graph:** articles stored in an array, citations stored in adjacency lists, with `degre_in` tracking received citations.
- **BFS:** explores the network level by level from a source article to simulate propagation.
- **Detection:** searches article titles for suspicious phrases and words (case-insensitive, using `strstr` and `tolower`). The reliability score is `100 - suspicion score`, with a minimum of 0. Articles are classified as `SUSPECT`, `DOUTEUX` or `FIABLE`.

## Input file format

Each line starts with a letter:
- `A` : create an article
- `C` : add a citation between two articles

See `reseau.txt` for an example.

## Compile and run

```bash
gcc *.c -o projet
./projet
```

You can also open `projet c 2.cbp` with Code::Blocks.

## Limitations

- Limited graph size
- Text-only interface
- No automatic saving

## Authors

- Rayen Khammassi
- Youssef Besbes

Supervised by Mme Dridi Kaouther and Mme Ibtissem Ben Ouhiba.
Institut Supérieur de Gestion, April 2026.
