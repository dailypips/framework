#include <samchon/library/StaticEventDispatcher.hpp>

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <samchon/library/CriticalMap.hpp>
#include <samchon/library/CriticalSet.hpp>
#include <samchon/library/Event.hpp>
#include <samchon/library/ErrorEvent.hpp>
#include <samchon/library/ProgressEvent.hpp>

using namespace std;
using namespace samchon::library;

/* -------------------------------------------------------------
	CONSTRUCTORS
------------------------------------------------------------- */
StaticEventDispatcher::StaticEventDispatcher() 
{
}
StaticEventDispatcher::StaticEventDispatcher(const StaticEventDispatcher &eventDispatcher)
{
	//DO NOT COPY EVENTS
}
StaticEventDispatcher::StaticEventDispatcher(StaticEventDispatcher &&eventDispatcher)
{
	//COPY EVENTS
}

/* -------------------------------------------------------------
	EVENT LISTENER IN & OUT
------------------------------------------------------------- */
void StaticEventDispatcher::addEventListener(int type, void(*listener)(std::shared_ptr<Event>))
{
	UniqueWriteLock uk(mtx);

	auto &set = eventSetMap[type];
	set.insert(listener);
}
void StaticEventDispatcher::removeEventListener(int type, void(*listener)(std::shared_ptr<Event>))
{
	UniqueWriteLock uk(mtx);

	if (eventSetMap.count(type) > 0 && eventSetMap[type].count(listener) > 0)
		eventSetMap[type].erase(listener);
}

/* -------------------------------------------------------------
	SEND EVENT
------------------------------------------------------------- */
auto StaticEventDispatcher::dispatchEvent(shared_ptr<Event> event) -> bool
{
	UniqueReadLock uk(mtx);

	int type = event->getType();
	if (eventSetMap.count(type) == 0 || eventSetMap[type].empty() == true)
		return false;

	auto &eventSet = eventSetMap[type];
	for (auto it = eventSet.begin(); it != eventSet.end(); it++)
	{
		thread([it, event, this]
		{
			UniqueAcquire u_ac(this->semaphore);

			(*it)(event);
		}).detach();
	}
	return true;
}
auto StaticEventDispatcher::dispatchProgressEvent(size_t x, size_t size) -> bool
{
	shared_ptr<ProgressEvent> event(new ProgressEvent(this, x, size));

	return dispatchEvent(event);
}

/*void StaticEventDispatcher::eventActivated()
{
	sendEvent(Event::ACTIVATE);
}
void StaticEventDispatcher::eventCompleted()
{
	sendEvent(Event::COMPLETE);
}
void StaticEventDispatcher::sendRemoved()
{
	if (eventSetMap.has(Event::REMOVED) == false)
		return;

	shared_ptr<Event> event(new Event(this, Event::REMOVED));
	auto eventSet = eventSetMap.get(event->getType());

	eventSet->readLock();
	for (auto it = eventSet->begin(); it != eventSet->end(); it++)
		(*it)(event);
	eventSet->readUnlock();
}

void StaticEventDispatcher::sendEvent(long type)
{
	if (type == Event::REMOVED)
	{
		sendRemoved();
		return;
	}
	else if (eventSetMap.has(type) == false)
		return;

	shared_ptr<Event> event(new Event(this, type));
	auto eventSet = eventSetMap.get(event->getType());

	eventSet->readLock();
	for (auto it = eventSet->begin(); it != eventSet->end(); it++)
		thread(*it, event).detach();
	eventSet->readUnlock();
}
void StaticEventDispatcher::sendError(long id)
{
	shared_ptr<ErrorEvent> event(new ErrorEvent(this, ErrorEvent::ERROR_OCCURED, id));

	errorSet.readLock();
	for (auto it = errorSet.begin(); it != errorSet.end(); it++)
		thread(*it, event).detach();
	errorSet.readUnlock();
}
void StaticEventDispatcher::sendProgress(unsigned long long x, unsigned long long size)
{
	shared_ptr<ProgressEvent> event(new ProgressEvent(this, x, size));

	progressSet.readLock();
	for (auto it = progressSet.begin(); it != progressSet.end(); it++)
		thread(*it, event).detach();
	progressSet.readUnlock();
}*/