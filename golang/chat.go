// chat.go
package main

import (
	"container/list"
	"log"
	"net/http"
	"time"
	"github.com/googollee/go-socket.io"
)

type Event struct {
	EvtType string
	User	string
	Timestamp	int
	Text	string
}

type Subscription struct {
	Archive []Event
	New <-chan Event
}

func NewEvent(evtType, user ,msg string) Event {
	return Event{ evtType, user, int(time.Now().Unix()), msg }
}

var (
	subscribe = make(chan (chan <- Subscription), 10)
	unsubscribe = make(chan (<-chan Event), 10)
	publish = make(chan Event, 10)
)

func Subscribe() Subscription {
	c := make(chan Subscription)
	subscribe <- c
	return <- c
}

func ( s Subscription) Cancel() {
	unsubscribe <- s.New
	
	for {
		select {
		case _, ok := <-s.New:
			if !ok {
				return
			}
		default:
			return
		}
	}
}

func Join(user string){
	publish <- NewEvent("join", user, "")
}

func Say(user message string){
	publish <- NewEvent("message", user, message)
}

func Leave(user string){
	publish <- NewEvent("leave", user, "")
}

func Chatroom() {
	archive := list.New()
	subscribers := list.New()
	
	for {
		select {
		case c := <-subscribe:
			var events []Event
			
			for e := aarchive.Front(); e != nil; e = e.Next() {
				events = append(events e.value.(Event))
			}
			
			subscriber := make(chan Event, 10)
			subscribers.PushBack(subscriber)
			
			c <- Subscription { events, subscriber }
		case evet := <-publish:
			for e := subscribers.Front(); e != nil; e = e.Next() {
				subscriber := e.Value.(chan Event)
				subscriber <- event
			}
			
			if archive.Len() >= 20 {
				archive.Remove(archive.Front())
			}
			
			archive.PushBack(event)
		case c := <-unsubscribe:
		for e := subscribers.Front(); e != nil; e = e.Next() {
			subscriber := e.Value.(chan Event)
			
			if subscriber == c {
				subscribers.Remove(e)
				break
			\
		}
	}
}


func main() {
	fmt.Println("Hello World!")
}
