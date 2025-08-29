#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <dirent.h>


struct Point {
    double x, y, z;
};

struct Element {
    int type;                 // 单元类型 (2=三角形)
    std::vector<int> nodes;   // 节点编号
};

int main() {
    std::ifstream fin("main.msh");
    std::string line;
    std::vector<Point> points;
    std::vector<Element> elements;

    while (std::getline(fin, line)) {
        if (line == "$Nodes") {
            // 读取节点
            int numEntityBlocks, numNodes, minNodeTag, maxNodeTag;
            fin >> numEntityBlocks >> numNodes >> minNodeTag >> maxNodeTag;

            points.resize(numNodes + 1); // 节点编号从1开始
            for (int b = 0; b < numEntityBlocks; b++) {
                int entityDim, entityTag, parametric, numNodesInBlock;
                fin >> entityDim >> entityTag >> parametric >> numNodesInBlock;

                std::vector<int> nodeTags(numNodesInBlock);
                for (int i = 0; i < numNodesInBlock; i++) fin >> nodeTags[i];

                for (int i = 0; i < numNodesInBlock; i++) {
                    double x, y, z;
                    fin >> x >> y >> z;
                    points[nodeTags[i]] = {x, y, z};
                }
            }
        }
        else if (line == "$Elements") {
            // 读取单元
            int numEntityBlocks, numElements, minElemTag, maxElemTag;
            fin >> numEntityBlocks >> numElements >> minElemTag >> maxElemTag;

            for (int b = 0; b < numEntityBlocks; b++) {
                int entityDim, entityTag, elemType, numElemsInBlock;
                fin >> entityDim >> entityTag >> elemType >> numElemsInBlock;

                for (int i = 0; i < numElemsInBlock; i++) {
                    int elemTag;
                    fin >> elemTag;
                    Element elem;
                    elem.type = elemType;

                    int numNodes = 0;
                    if (elemType == 2) numNodes = 3; // 三角形
                    else if (elemType == 4) numNodes = 4; // 四面体
                    // 其他类型可以扩展

                    elem.nodes.resize(numNodes);
                    for (int j = 0; j < numNodes; j++) {
                        fin >> elem.nodes[j];
                    }
                    elements.push_back(elem);
                }
            }
        }
    }

    fin.close();

    // 输出调试
    std::cout << "读取到 " << points.size()-1 << " 个点" << std::endl;
    std::cout << "读取到 " << elements.size() << " 个单元" << std::endl;

    for (size_t i = 0; i < elements.size(); i++) {
        if (elements[i].type == 2) { // 三角形
            std::cout << "三角形单元: ";
            for (int n : elements[i].nodes) {
                std::cout << "(" << points[n].x << "," << points[n].y << "," << points[n].z << ") ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
