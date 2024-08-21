#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <QLayout>
#include <QWidget>

int getClosestInt(int a);

std::pair<int, int> getBorderByThreshold(std::vector<int> data, int threshold);

void clearLayout(QLayout *layout);

#endif