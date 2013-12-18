/*
 * Copyright (c) 2011, 2012, 2013 Nicira, Inc.
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

#ifndef SSET_H
#define SSET_H

#include "hmap.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sset_node {
    struct hmap_node hmap_node;
    char name[1];
};

/* A set of strings. */
struct sset {
    struct hmap map;
};

#define SSET_INITIALIZER(SSET) { HMAP_INITIALIZER(&(SSET)->map) }

/* Basics. */
static inline void sset_init(struct sset *);
void sset_destroy(struct sset *);
void sset_clone(struct sset *, const struct sset *);
static inline void sset_swap(struct sset *, struct sset *);
static inline void sset_moved(struct sset *);

/* Initializes 'set' as an empty set of strings. */
static inline void
sset_init(struct sset *set)
{
    hmap_init(&set->map);
}

/* Exchanges the contents of 'a' and 'b'. */
static inline void
sset_swap(struct sset *a, struct sset *b)
{
    hmap_swap(&a->map, &b->map);
}

/* Adjusts 'set' so that it is still valid after it has been moved around in
 * memory (e.g. due to realloc()). */
static inline void
sset_moved(struct sset *set)
{
    hmap_moved(&set->map);
}

/* Count. */
static inline bool sset_is_empty(const struct sset *);
static inline size_t sset_count(const struct sset *);


/* Returns true if 'set' contains no strings, false if it contains at least one
 * string. */
static inline bool
sset_is_empty(const struct sset *set)
{
    return hmap_is_empty(&set->map);
}

/* Returns the number of strings in 'set'. */
static inline size_t
sset_count(const struct sset *set)
{
    return hmap_count(&set->map);
}

/* Insertion. */
struct sset_node *sset_add(struct sset *, const char *);
struct sset_node *sset_add_and_free(struct sset *, char *);
void sset_add_assert(struct sset *, const char *);
void sset_add_array(struct sset *, char **, size_t n);

/* Deletion. */
void sset_clear(struct sset *);
void sset_delete(struct sset *, struct sset_node *);
bool sset_find_and_delete(struct sset *, const char *);
void sset_find_and_delete_assert(struct sset *, const char *);
char *sset_pop(struct sset *);

/* Search. */
struct sset_node *sset_find(const struct sset *, const char *);

/* Returns true if 'set' contains a copy of 'name', false otherwise. */
static inline bool
sset_contains(const struct sset *set, const char *name)
{
    return sset_find(set, name) != NULL;
}

bool sset_equals(const struct sset *, const struct sset *);
struct sset_node *sset_at_position(const struct sset *,
                                   uint32_t *bucketp, uint32_t *offsetp);

/* Iteration macros. */
#define SSET_FOR_EACH(NAME, SSET)               \
    for ((NAME) = SSET_FIRST(SSET);             \
         NAME != NULL;                          \
         (NAME) = SSET_NEXT(SSET, NAME))

#define SSET_FOR_EACH_SAFE(NAME, NEXT, SSET)        \
    for ((NAME) = SSET_FIRST(SSET);                 \
         (NAME != NULL                              \
          ? (NEXT) = SSET_NEXT(SSET, NAME), true    \
          : false);                                 \
         (NAME) = (NEXT))

const char **sset_sort(const struct sset *);

/* Implementation helper macros. */

#define SSET_NODE_FROM_HMAP_NODE(HMAP_NODE) \
    CONTAINER_OF(HMAP_NODE, struct sset_node, hmap_node)
#define SSET_NAME_FROM_HMAP_NODE(HMAP_NODE) \
    HMAP_NODE == NULL                       \
    ? NULL                                  \
    : (CONST_CAST(const char *, (SSET_NODE_FROM_HMAP_NODE(HMAP_NODE)->name)))
#define SSET_NODE_FROM_NAME(NAME) CONTAINER_OF(NAME, struct sset_node, name)
#define SSET_FIRST(SSET) SSET_NAME_FROM_HMAP_NODE(hmap_first(&(SSET)->map))
#define SSET_NEXT(SSET, NAME)                                           \
    SSET_NAME_FROM_HMAP_NODE(                                           \
        hmap_next(&(SSET)->map, &SSET_NODE_FROM_NAME(NAME)->hmap_node))

#ifdef __cplusplus
}
#endif

#endif /* sset.h */
