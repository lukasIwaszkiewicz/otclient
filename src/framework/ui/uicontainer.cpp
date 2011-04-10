/* The MIT License
 *
 * Copyright (c) 2010 OTClient, https://github.com/edubart/otclient
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "uicontainer.h"
#include "core/resources.h"
#include "uibutton.h"
#include "uipanel.h"
#include "uilabel.h"
#include "uitextedit.h"
#include "uiwindow.h"

UIContainerPtr rootContainer(new UIContainer);

UIContainerPtr& UIContainer::getRootContainer()
{
    return rootContainer;
}

void UIContainer::addChild(UIElementPtr child)
{
    m_children.push_back(child);
    child->setParent(asUIContainer());
}

void UIContainer::removeChild(UIElementPtr child)
{
    if(m_activeElement == child)
        setActiveElement(UIElementPtr());
    m_children.remove(child);
    if(child->getParent() == shared_from_this())
        child->setParent(UIContainerPtr());
}

UIElementPtr UIContainer::getChildById(const std::string& id)
{
    if(getId() == id)
        return asUIElement();
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        if((*it)->getId() == id) {
            return (*it);
        }
    }
    return UIElementPtr();
}

UIElementPtr UIContainer::recursiveGetChildById(const std::string& id)
{
    if(getId() == id)
        return asUIElement();

    UIElementPtr element;
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        element = (*it);
        if(element->getId() == id) {
            return element;
        } else {
            UIContainerPtr container = element->asUIContainer();
            if(container) {
                element = container->recursiveGetChildById(id);
                if(element)
                    return element;
            }
        }
    }
    return UIElementPtr();
}

void UIContainer::render()
{
    UIElement::render();
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        const UIElementPtr& child = (*it);
        if(child->isVisible())
            child->render();
    }
}

bool UIContainer::onInputEvent(const InputEvent& event)
{
    bool ret = false;
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        const UIElementPtr& child = (*it);
        if(child->isEnabled() && child->isVisible())
            ret = child->onInputEvent(event) || ret;
    }
    return ret;
}

void UIContainer::setActiveElement(UIElementPtr activeElement)
{

}
