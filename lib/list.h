/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2013 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LIST_H
#define LIST_H 1

/* Doubly linked list. */

#include <stdbool.h>
#include <stddef.h>
#include "util.h"

/* Doubly linked list head or element. */
struct list {
    struct list *prev;     /* Previous list element. */
    struct list *next;     /* Next list element. */
};

#define LIST_INITIALIZER(LIST) { LIST, LIST }

/* Initializes 'list' as an empty list. */
static inline void
list_init(struct list *list)
{
    list->next = list->prev = list;
}

/* Initializes 'list' with pointers that will (probably) cause segfaults if
 * dereferenced and, better yet, show up clearly in a debugger. */
static inline void
list_poison(struct list *list)
{
    memset(list, 0xcc, sizeof *list);
}

/* List insertion. */
void list_insert(struct list *, struct list *);
void list_splice(struct list *before, struct list *first, struct list *last);
void list_replace(struct list *, const struct list *);
void list_moved(struct list *);
void list_move(struct list *dst, struct list *src);

/* Inserts 'elem' at the beginning of 'list', so that it becomes the front in
   'list'. */
static inline void
list_push_front(struct list *list, struct list *elem)
{
    list_insert(list->next, elem);
}

/* Inserts 'elem' at the end of 'list', so that it becomes the back in
 * 'list'. */
static inline void
list_push_back(struct list *list, struct list *elem)
{
    list_insert(list, elem);
}

/* List removal. */
struct list *list_remove(struct list *);
struct list *list_pop_front(struct list *);
struct list *list_pop_back(struct list *);

/* List elements. */
struct list *list_front(const struct list *);
struct list *list_back(const struct list *);

/* List properties. */
size_t list_size(const struct list *);

/* Returns true if 'list' is empty, false otherwise. */
static inline bool
list_is_empty(const struct list *list)
{
    return list->next == list;
}

/* Returns true if 'list' has 0 or 1 elements, false otherwise. */
static inline bool
list_is_short(const struct list *list)
{
    return list->next == list->prev;
}

/* Returns true if 'list' has exactly 1 element, false otherwise. */
static inline bool
list_is_singleton(const struct list *list)
{
    return list_is_short(list) && !list_is_empty(list);
}

#define LIST_FOR_EACH(ITER, MEMBER, LIST)                               \
    for (ASSIGN_CONTAINER(ITER, (LIST)->next, MEMBER);                  \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.next, MEMBER))
#define LIST_FOR_EACH_CONTINUE(ITER, MEMBER, LIST)                      \
    for (ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.next, MEMBER);           \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.next, MEMBER))
#define LIST_FOR_EACH_REVERSE(ITER, MEMBER, LIST)                       \
    for (ASSIGN_CONTAINER(ITER, (LIST)->prev, MEMBER);                  \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.prev, MEMBER))
#define LIST_FOR_EACH_REVERSE_CONTINUE(ITER, MEMBER, LIST)              \
    for (ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.prev, MEMBER);           \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.prev, MEMBER))
#define LIST_FOR_EACH_SAFE(ITER, NEXT, MEMBER, LIST)               \
    for (ASSIGN_CONTAINER(ITER, (LIST)->next, MEMBER);             \
         (&(ITER)->MEMBER != (LIST)                                \
          ? ASSIGN_CONTAINER(NEXT, (ITER)->MEMBER.next, MEMBER), 1 \
          : 0);                                                    \
         (ITER) = (NEXT))

#endif /* list.h */
