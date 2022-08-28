/*
 poker.c
 
 Use a struct to define a card as an enumerated member that is its suit value and a short that is its pips value.
 Write a function that randomly shuffles the deck.
 Submit your work as a text file.
 Then deal out 7 card hands and evaluate the probability that a hand has no pair, one pair, two pair, three of a kind, full house and 4 of a kind.  This is a Monte Carlo method to get an approximation to these probabilities.  Use at least 1 million randomly generated hands.
 You can check against probabilities found in a standard table.
 
 Created by Ganèche Ramanoudjame on 17/08/2022.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EMPTY -1
#define DECK_SIZE 52
#define HAND_SIZE 7
#define PIPS_SIZE 13
#define SUIT_SIZE 4
#define RANK_SIZE 10
#define MONTE_CARLO_TRIES 1000000

// Constants for printing purpose
static const char PIPS_CHAR[] = "23456789TJQKA";
static const char *SUIT_UTF8[] = {"\u2660", "\u2665", "\u2666", "\u2663"};
static const char *RANK[] = {"Ace high of less", "Pair", "Two pair", "Three of a kind", "Straight", "Flush", "Full house", "Four of a kind", "Straight flush", "Royal flush"};
static const long double PROBABILITIES[] = {0.17411920, 0.43822546, 0.23495536, 0.04829870, 0.04619382, 0.03025494, 0.02596102, 0.00168067, 0.00027851, 0.00003232};

// Pips is an enum from TWO to ACE
typedef enum pips {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE} pips;
// Suit is an enum from SPADE to CLUB
typedef enum suit {SPADE, HEART, DIAMOND, CLUB} suit;
// Rank from NO_PAIR TO ROYAL_FLUSH
typedef enum rank {ACE_HIGH_OR_LESS, PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH} rank;

// Card is a struct of pips and suit
typedef struct card {
    pips pips;
    suit suit;
}card;

// Stack of maximum DECK_SIZE cards
typedef struct stack {
    card cards[DECK_SIZE];
    short top;
}stack;

// Declaration of functions
void printStack(const stack*);
void push(card, stack*);
card pop(stack*);
void reset(stack*);
void fillDeck(stack*);
void swapCards(card*, card*);
void shuffle(stack*);
void deal(stack*, stack*, int);
rank evalBestPokerRank(const stack *);

// Print value of card
void printCard(const card c){
    printf("%c%s", PIPS_CHAR[c.pips], SUIT_UTF8[c.suit]);
}

// Print the cards in the stack
void printStack(const stack *s){
    for(int i = 0; i <= s->top; i++){
        printCard(s->cards[i]);
        printf(" ");
    }
    printf("\n");
}

// Push a card on top of stack
void push(card c, stack *s){
    s->top++;
    s->cards[s->top]=c;
}

// Pop a card from top of stack
card pop(stack *s){
    return(s->cards[s->top--]);
}

// Reset stack
void reset(stack *s){
    s->top = EMPTY;
}

// Fill deck with 52 new cards
void fillDeck(stack *stk){
    for(suit s = SPADE; s <= CLUB; s++) {
        for(pips p=TWO; p <= ACE; p++){
            card c = {p, s};
            push(c, stk);
        }
    }
}

// Swap two cards
void swapCards(card *a, card *b){
    card temp = *a;
    *a = *b;
    *b = temp;
}

// Shuffle a stack of cards using Fisher-Yates shuffle algorithm
void shuffle(stack *s){
    for(int i = s->top;  i > 0; i--) {
        int random = rand() % (i+1);
        swapCards(&s->cards[i], &s->cards[random]);
    }
}

// Deal from source stack to destination stack
void deal(stack *source, stack *destination, int size){
    for(int i=0; i<size; i++){
        push(pop(source), destination);
    }
}

// Evaluate the best poker rank in a stack
rank evalBestPokerRank(const stack *hand){
    rank result = ACE_HIGH_OR_LESS;
    int pipsHistogram[PIPS_SIZE] = {0};
    int pipsHistogramOfHistogram[SUIT_SIZE+1] = {0};
    int suitHistogram[SUIT_SIZE] = {0};
    int suitHistogramOfHistogram[PIPS_SIZE+1] = {0};
    
    // Prepare some KPI
    for(int i=0; i <= hand->top; i++){
        pipsHistogram[hand->cards[i].pips]++;
        suitHistogram[hand->cards[i].suit]++;
    }
    for(pips p = TWO; p <= ACE; p++){
        pipsHistogramOfHistogram[pipsHistogram[p]]++;
    }
    for(suit s= SPADE; s <= CLUB; s++){
        suitHistogramOfHistogram[suitHistogram[s]]++;
    }
    
    int isFlush = suitHistogramOfHistogram[5] || suitHistogramOfHistogram[6] || suitHistogramOfHistogram[7] || suitHistogramOfHistogram[8]|| suitHistogramOfHistogram[9]|| suitHistogramOfHistogram[10]|| suitHistogramOfHistogram[11]|| suitHistogramOfHistogram[12]|| suitHistogramOfHistogram[13];
    
    int inARow = 0, isStraight = 0;
    for (pips p = ACE; !isStraight && (p != TWO - 1); p--){
        inARow = pipsHistogram[p] ? inARow + 1 : 0;
        isStraight = (inARow > 4);
    }
    inARow = pipsHistogram[ACE] ? inARow + 1 : 0;
    isStraight = isStraight || (inARow > 4); // One more time for the wheel
    
    // Evaluate bestHand from top to down
    if (isFlush && isStraight){
        // then worth to check if the straight is a straight flush or a royal flush
        int suitedPipsHistogram[SUIT_SIZE][PIPS_SIZE] = {0};
        for(int i=0; i <= hand->top; i++){
            suitedPipsHistogram[hand->cards[i].suit][hand->cards[i].pips]++;
        }
        int isRoyalFlush = 0;
        for(suit s = SPADE; !isRoyalFlush && (s <= CLUB); s++){
            int suitedInARow = 0, isStraightFlush = 0;
            for (pips p = ACE; !isStraightFlush && (p != TWO-1); p--){
                suitedInARow = suitedPipsHistogram[s][p] ? suitedInARow + 1 : 0;
                isStraightFlush = (suitedInARow > 4);
                isRoyalFlush = isStraightFlush && (p == TEN);
            }
            suitedInARow = suitedPipsHistogram[s][ACE] ? suitedInARow + 1 : 0;
            isStraightFlush = isStraightFlush || (suitedInARow > 4); // One more time for the wheel flush
            result = isRoyalFlush ? ROYAL_FLUSH : result;
            result = !result && isStraightFlush ? STRAIGHT_FLUSH : result;
        }
    }
    result = !result && pipsHistogramOfHistogram[4] ? FOUR_OF_A_KIND : result;
    result = !result && pipsHistogramOfHistogram[3] && pipsHistogramOfHistogram[2] ? FULL_HOUSE : result;
    result = !result && isFlush ? FLUSH : result;
    result = !result && isStraight ? STRAIGHT : result;
    result = !result && pipsHistogramOfHistogram[3] ? THREE_OF_A_KIND : result;
    result = !result && (pipsHistogramOfHistogram[2] > 1) ? TWO_PAIR : result;
    result = !result && pipsHistogramOfHistogram[2] ? PAIR : result;
    return result;
}

int main(int argc, const char * argv[]) {
    // Declaration
    stack deck, hand;
    unsigned long result[RANK_SIZE] = {0};
    
    // Initialisation
    srand((int)time(NULL));
    reset(&deck);
    reset(&hand);
    
    // Experiment : shuffle, deal and evaluate multiple times
    fillDeck(&deck);
    for (unsigned long i = 0; i < MONTE_CARLO_TRIES; i++){
        shuffle(&deck);
        deal(&deck, &hand, HAND_SIZE);
        rank r = evalBestPokerRank(&hand);
        result[r]++;
        // Print hand for checking purpose
        // printf("%lu %s : ", i, RANK[rank]);
        // printStack(&hand);
        deal(&hand, &deck, HAND_SIZE); // return cards from hand to deck
    }
    
    // Print result
    printf("\n%s  %s  %s      : %s", "MonteCarlo", "Probability", "Accuracy", "Rank");
    printf("\n%s", "--------------------------------------------------------");
    for(int i = 0; i <= ROYAL_FLUSH; i++){
        long double montecarlo = (long double) result[i]/MONTE_CARLO_TRIES;
        long double accuracy = 100*(1 - fabsl(1 - montecarlo/PROBABILITIES[i]));
        printf("\n%Lf    %Lf     %Lf %s   : %s", montecarlo, PROBABILITIES[i], accuracy, "%", RANK[i]);
    }
    printf("\n\n");
    return 0;
}
