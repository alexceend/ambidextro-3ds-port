#include <iostream>
#include <list>
#include <string>
#include <map>
#include <list>

enum struct EventType
{
    WIN,
    PUASE,
    DEATH,
    TIME_OUT
};

class IObserver
{
    public:
        virtual ~IObserver(){};
        virtual void Update(const std::string &message_from_subject) = 0;
};

class ISubject
{
    public:
        virtual ~ISubject(){};
        virtual void Subscribe(EventType eventType, IObserver* observer) = 0;
        virtual void Unsubscribe(EventType eventType, IObserver* observer) = 0;
        virtual void Notify(EventType eventType) = 0;
};

class Subject : public ISubject
{
    public:
        virtual ~Subject()
        {
            std::cout << "Test Subject";
        }

        void Subscribe(EventType _EventType, IObserver* observer) override
        {
            bool subscribed = false;
            std::list<IObserver*>::iterator it = observers[_EventType].begin();
            while (it != observers[_EventType].end())
            {
                if (*it == observer)
                {
                    subscribed = true;
                    break;
                }
                else it++;
            }
            if (!subscribed) observers[_EventType].push_back(observer);
        }

        void Unsubscribe(EventType _eventType, IObserver* observer) override
        {
            std::list<IObserver*>::iterator it = observers[_eventType].begin();
            while (it != observers[_eventType].end())
            {
                if (*it == observer)
                {
                    it = observers[_eventType].erase(it);
                }
                else it++;
            }
        }

        void Notify(EventType _eventType) override
        {
            std::list<IObserver*>::iterator it = observers[_eventType].begin();
            while (it != observers[_eventType].end())
            {
                Notify(_eventType);
            }
        }
    private:
        std::map<EventType, std::list<IObserver*>> observers;
};