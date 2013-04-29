#include "mdsparametertablemodel.h"

parameterModel::parameterModel(QWidget *parent) : QTreeWidget(parent)
{
    this->root_ = new parameterNode("parameters","");
    this->root_->parent = 0;
    this->addTopLevelItem(this->root());
    this->setColumnCount(2);
    this->hide();
}

parameterModel::~parameterModel()
{
    qDebug() << "Deleting Parameter Mode";
    this->freeNodeMemory(this->root_);
}

void parameterModel::freeNodeMemory(parameterNode *node)
{
    if (node->children.count()==0)
    {
        delete node;
        return;
    }
    else
    {
        QHash<QString, parameterNode*>::iterator i = node->children.begin();
        while (i != node->children.end())
        {
            this->freeNodeMemory(i.value());
            ++i;
        }
    }
    delete node;
}

void parameterModel::addNode(QString pathName, QString value,
                             parameterNode *parent)
{
    // Current parent
    parameterNode *currentParent;

    // Parse the path name
    int tokenIndex = pathName.indexOf("$");

    // Check if current path level is equal to parent
    QString parentPathLevel = pathName.left(tokenIndex);
    if(parent->name() != parentPathLevel)
    {
        qDebug() << "parent mismatch";
        return;
    }

    // Truncate path
    QString truncatedPath = pathName.right(pathName.size()-tokenIndex-1);

    // Get current level
    tokenIndex = truncatedPath.indexOf("$");

    QString currentPathLevel = truncatedPath.left(tokenIndex);

    // We are only writing node to leaf, and empty elsewhere
    QString valueStr;

    if (tokenIndex == -1)
        valueStr = value;

    // Check to see if is a child exists
    if (parent->children.contains(currentPathLevel))
    {
        // If the level already exists we simply pass down that branch
        currentParent = parent->children.value(currentPathLevel);
    }
    else
    {
        // If level doesn't exist, we need to create a new node here
        parameterNode *newNode = new parameterNode(currentPathLevel,valueStr,
                                                   parent);
        newNode->parent = parent;
        parent->children.insert(currentPathLevel,newNode);
        parent->addChild(newNode);
        currentParent = newNode;
    }

    // Stop recursion if we have reached last token
    if (tokenIndex == -1)
        return;

    // Recurse
    this->addNode(truncatedPath,value,currentParent);
}
