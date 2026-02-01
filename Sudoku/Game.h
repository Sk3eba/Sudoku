#pragma once

#include <array>
#include <SFML/System.hpp>

/**
 * @class Game
 * @brief Reprezentuje stan logiczny planszy Sudoku.
 *
 * Klasa ta zarz¹dza siatk¹ 9x9, przechowuje wartoœci w komórkach,
 * obs³uguje "o³ówkowe" notatki (kandydatów) oraz weryfikuje poprawnoœæ rozwi¹zania.
 */
class Game
{
public:
    /**
     * @brief Konstruktor domyœlny.
     *
     * Inicjalizuje pust¹ planszê i czyœci wszystkich kandydatów.
     */
    Game();

    /**
     * @brief Czyœci ca³¹ planszê.
     *
     * Ustawia wartoœci wszystkich komórek na 0 (puste) oraz usuwa wszystkie notatki.
     */
    void clear();

    /**
     * @brief Ustawia wartoœæ w danej komórce.
     *
     * @param cell Wspó³rzêdne komórki (x, y).
     * @param value Wartoœæ do wpisania (1-9) lub 0, aby wyczyœciæ komórkê.
     * @return true Jeœli ustawienie siê powiod³o (poprawne wspó³rzêdne i zakres wartoœci).
     * @return false W przypadku b³êdu.
     */
    bool setCell(sf::Vector2i cell, int value);

    /**
     * @brief Pobiera wartoœæ z danej komórki.
     *
     * @param cell Wspó³rzêdne komórki (x, y).
     * @return int Wartoœæ komórki (1-9, lub 0 jeœli pusta). Zwraca -1 w przypadku b³êdnych wspó³rzêdnych.
     */
    int getCell(sf::Vector2i cell) const;

    /**
     * @brief Sprawdza, czy wstawienie danej liczby w dane miejsce jest poprawnym ruchem.
     *
     * Weryfikuje, czy liczba nie powtarza siê ju¿ w wierszu, kolumnie lub bloku 3x3.
     * Nie modyfikuje planszy.
     *
     * @param cell Wspó³rzêdne komórki.
     * @param value Liczba do sprawdzenia (1-9).
     * @return true Jeœli ruch jest zgodny z zasadami Sudoku.
     * @return false Jeœli ruch narusza zasady.
     */
    bool isValidMove(sf::Vector2i cell, int value) const;

    /**
     * @brief Sprawdza poprawnoœæ ca³ej planszy.
     *
     * Weryfikuje wszystkie wiersze, kolumny i bloki pod k¹tem duplikatów.
     *
     * @return true Jeœli plansza jest w stanie poprawnym (brak konfliktów).
     * @return false Jeœli znaleziono b³¹d.
     */
    bool isBoardValid() const;

    /**
     * @brief Sprawdza, czy gra zosta³a zakoñczona sukcesem.
     *
     * Gra jest rozwi¹zana, jeœli plansza jest wype³niona (brak zer) i nie zawiera b³êdów.
     *
     * @return true Jeœli Sudoku jest rozwi¹zane.
     */
    bool isSolved() const;

    /**
     * @brief Udostêpnia surow¹ tablicê wartoœci planszy.
     *
     * @return const std::array<int, 81>& Referencja do tablicy 81 liczb ca³kowitych.
     */
    const std::array<int, 81>& rawBoard() const { return m_cells; }

    /**
     * @brief Prze³¹cza stan kandydata (notatki) w danej komórce.
     *
     * Jeœli liczba by³a zaznaczona, zostanie odznaczona i vice versa.
     *
     * @param cell Wspó³rzêdne komórki.
     * @param number Liczba kandyduj¹ca (1-9).
     */
    void toggleCandidate(sf::Vector2i cell, int number);

    /**
     * @brief Sprawdza, czy dany kandydat jest zaznaczony w komórce.
     *
     * @param cell Wspó³rzêdne komórki.
     * @param number Liczba do sprawdzenia.
     * @return true Jeœli notatka dla tej liczby jest aktywna.
     */
    bool hasCandidate(sf::Vector2i cell, int number) const;

    /**
     * @brief Usuwa wszystkie notatki (kandydatów) z danej komórki.
     *
     * @param cell Wspó³rzêdne komórki.
     */
    void clearCandidates(sf::Vector2i cell);

private:
    std::array<int, 81> m_cells; ///< Przechowuje stan planszy (indeksowane 0-80).
    std::array<std::array<bool, 10>, 81> m_candidates; ///< Przechowuje notatki dla ka¿dej komórki.

    static bool inRangeCell(sf::Vector2i cell);
    static int indexFromCell(sf::Vector2i cell);

    // Helpers for board validation
    bool checkRowValid(int row) const;
    bool checkColValid(int col) const;
    bool checkBlockValid(int blockRow, int blockCol) const;
};