#ifndef __ADJ_LIST__
#define __ADJ_LIST__

#include <stddef.h>
#include <stdbool.h>


#define circularAdjList_add( type_in, circularAdjList, data, copier, comparator) \
  ((type_in*) circularAdjList_add_( ( struct AdjList *) circularAdjList, NULL, sizeof( type_in ), ( void *) data, copier, comparator))

#define circularAdjList_search( type_in, circularAdjList, data, comparator) \
  ((type_in*) circularAdjList_search_( ( struct AdjList *) circularAdjList, ( void *) &data, comparator))

#define circularAdjList_free( type_in, circularAdjList, freerFunction) \
  circularAdjList = ((type_in*) circularAdjList_free_( ( struct AdjList *) circularAdjList, freerFunction ))

/// ///

#define circularAdjListStruct_header( structName ) \
  struct structName

#define circularAdjListStruct_content( structName ) \
  struct structName * next, * prev; \
  char isEnd;

/// ///

#define circularAdjListStaticDecl_struct( structName, varName ) \
  static struct structName * varName = NULL;

/// ///

#define circularAdjListCopier_header( functionName ) \
  void functionName( struct AdjList * circularAdjList, void * circularAdjListData )

#define circularAdjListComparator_header( functionName ) \
  bool functionName( struct AdjList * circularAdjList, void * circularAdjListData )

#define circularAdjListFreer_header( functionName ) \
  void functionName( struct AdjList * circularAdjList )

/// ///

#define circularAdjListTools_headCast( listType ) \
  listType * head = ( listType * ) circularAdjList;

#define circularAdjListTools_contentCast( listType, dataType ) \
  circularAdjListTools_headCast(listType) \
  dataType * data = ( dataType * ) circularAdjListData;

/// ///

#define circularAdjList_isEnd( circularAdjListPtrVar ) (circularAdjListPtrVar->isEnd==1)

#define circularAdjList_doWhileNext( circularAdjList ) \
  (circularAdjList = circularAdjList_isEnd(circularAdjList) ? NULL : circularAdjList->next )

/// ///


circularAdjListStruct_header( AdjList )
{
  circularAdjListStruct_content( AdjList )
};

typedef void ( *circularAdjListCopier_pf )(  struct AdjList * circularAdjList, void * dataToCopy );
typedef bool ( *circularAdjListComparator_pf )( struct AdjList * circularAdjList, void * dataToCompare );
typedef void ( *circularAdjListFreer_pf )( struct AdjList * circularAdjList );

struct AdjList * circularAdjList_add_( struct AdjList * circularAdjList, struct AdjList * previous_circularAdjList, const size_t sizeofList, void * dataToCopy, circularAdjListCopier_pf inserterFunction, circularAdjListComparator_pf comparatorFunction );
struct AdjList * circularAdjList_search_( struct AdjList * circularAdjList, void * dataToSearch, circularAdjListComparator_pf comparatorFunction );
struct AdjList * circularAdjList_free_( struct AdjList * circularAdjList, circularAdjListFreer_pf freerFunction );


#endif
