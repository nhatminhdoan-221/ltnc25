#include <ctime>
#include <random>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "queue.h"
#include "constants.h"

void Queue::generateNewBag() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(bag.begin(), bag.end(), g);
    for (int i : bag) {
        pieceQueue.push_back(i);
    }
}

void Queue::previewQueueUpdate() {
    previewQueue.clear();

    while (pieceQueue.size() < PIECE_PREVIEW) {
        generateNewBag();
    }

    for (int i = 0; i < PIECE_PREVIEW; i++) {
        if (pieceQueue.empty()) generateNewBag();
        previewQueue.push_back(pieceQueue.at(i)); //lay manh i cua pieceQ
        // them vao prevQ = > tao danh sach manh hien thi truoc
    }

    pieceQueue.erase(pieceQueue.begin()); // xoa manh ma nguoi coi se duoc nhan tiep theo
}

int Queue::getNextPiece() {
    if (pieceQueue.empty() && previewQueue.empty()) {
        std::cerr << "Cannot find next piece";
        exit(-1);
    }

    int nextPiece = previewQueue.at(0); // lay manh dau cua preview
    previewQueueUpdate(); 
    return nextPiece; //tra ve manh tiep theo
}