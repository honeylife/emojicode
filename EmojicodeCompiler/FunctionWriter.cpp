//
//  FunctionWriter.cpp
//  Emojicode
//
//  Created by Theo Weidmann on 28/09/2016.
//  Copyright © 2016 Theo Weidmann. All rights reserved.
//

#include "FunctionWriter.hpp"
#include "Token.hpp"
#include <cmath>

void FunctionWriter::writeInstruction(EmojicodeInstruction value) {
    instructions_.push_back(value);
}

void FunctionWriter::writeInstruction(std::initializer_list<EmojicodeInstruction> values) {
    for (auto value : values) {
        instructions_.push_back(value);
    }
}

void FunctionWriter::writeDoubleCoin(double val) {
    int exp = 0;
    double norm = std::frexp(val, &exp);
    int_least64_t scale = norm*PORTABLE_INTLEAST64_MAX;

    writeInstruction(scale >> 32);
    writeInstruction(static_cast<EmojicodeInstruction>(scale));
    writeInstruction(exp);
}

FunctionWriterPlaceholder FunctionWriter::writeInstructionPlaceholder() {
    writeInstruction(0);
    return FunctionWriterPlaceholder(this, instructions_.size() - 1);
}

FunctionWriterCountPlaceholder FunctionWriter::writeInstructionsCountPlaceholderCoin() {
    writeInstruction(0);
    return FunctionWriterCountPlaceholder(this, instructions_.size() - 1, count());
}

FunctionWriterInsertionPoint FunctionWriter::getInsertionPoint() {
    return FunctionWriterInsertionPoint(this, instructions_.size());
}

void FunctionWriterPlaceholder::write(EmojicodeInstruction value) const {
    writer_->instructions_[index_] = value;
}

void FunctionWriterCountPlaceholder::write() {
    FunctionWriterPlaceholder::write(static_cast<EmojicodeInstruction>(writer_->count() - count_));
}

void FunctionWriterInsertionPoint::insert(EmojicodeInstruction value) {
    writer_->instructions_.insert(writer_->instructions_.begin() + index_, value);
    index_++;
}

void FunctionWriterInsertionPoint::insert(std::initializer_list<EmojicodeInstruction> values) {
    writer_->instructions_.insert(writer_->instructions_.begin() + index_, values);
    index_ += values.size();
}
