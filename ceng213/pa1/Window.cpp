#include "Window.h"

Window::Window() {
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab()
{
    if(activeTab==-1)
    {
        return Tab();
    }
    else
    {
        return (tabs.getNodeAtIndex(activeTab)->data);
    }
}

bool Window::isEmpty() const
{
    return activeTab==-1;
}

void Window::newTab(const Tab &tab)
{
    tabs.insertAtIndex(tab,activeTab+1);
    activeTab=activeTab+1;
}

void Window::closeTab()
{
    if(activeTab != -1)
    {
        tabs.removeNodeAtIndex(activeTab);
        if (tabs.getSize()==0)
        {
            activeTab=-1;
        }
        else if(activeTab==tabs.getSize())
        {
            activeTab--;
        }
    }
}
void Window::moveActiveTabTo(int index)
{
    tabs.moveToIndex(activeTab,index);
    if(index>-1 && index<tabs.getSize()) activeTab=index;
    if(index>=tabs.getSize()) activeTab = tabs.getSize()-1;
}

void Window::changeActiveTabTo(int index)
{
    if(index<tabs.getSize() && index>-1) activeTab=index;
}

void Window::addTab(Node<Tab> &tab)
{
    tabs.append(tab.data);
    if(activeTab==-1) activeTab=0;
}

void Window::print() {
    if(tabs.isEmpty()) std::cout << "Window is empty" << std::endl;
    else std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}
