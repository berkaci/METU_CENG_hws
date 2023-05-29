#include "Browser.h"

Browser::Browser()
{
    windows.append(Window());
}

void Browser::newWindow()
{
    windows.prepend(Window());
}

void Browser::closeWindow()
{
    windows.removeNodeAtIndex(0);
}

void Browser::switchToWindow(int index)
{
    windows.moveToIndex(index,0);
}

Window &Browser::getWindow(int index)
{
    return windows.getNodeAtIndex(index) -> data;
}

void Browser::moveTab(Window &from, Window &to)
{
    Tab tmp=from.getActiveTab();
    from.closeTab();
    to.newTab(tmp);
}

void Browser::mergeWindows(Window &window1, Window &window2)
{
    while(!window2.isEmpty())
    {
        window2.changeActiveTabTo(0);
        window1.newTab(window2.getActiveTab());
        window2.closeTab();
    }
}

void Browser::mergeAllWindows()
{
    int i,si;
    si=windows.getSize();
    for(i=1;i<si;i++)
    {
        mergeWindows(windows.getFirstNode()->data,windows.getNodeAtIndex(i)->data);
    }
}

void Browser::closeAllWindows()
{
    windows.removeAllNodes();
}

void Browser::closeEmptyWindows()
{

    int i, si;
    si=windows.getSize();
    for(i=0;i<si;)
    {
        if(windows.getNodeAtIndex(i)->data.isEmpty())
        {
            windows.removeNodeAtIndex(i);
            si--;
        }
        else
        {
            i++;
        }
    }
}

void Browser::print() {
    Node<Window> *head = windows.getFirstNode();
    if(head == NULL) {
        std::cout << "The browser is empty" << std::endl;
    } else {
        (head -> data).print();
        head = head -> next;
        while(head != windows.getFirstNode()) {
            (head -> data).print();
            head = head -> next;
        }
    }
}
