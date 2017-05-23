/**
 * @author Christophe Sonntag
 */

#include "circularAdjList.h"

#include <stdlib.h>
#include <stdio.h>

struct AdjList * circularAdjList_add_( struct AdjList * circleAdjList, struct AdjList * previous_circleAdjList, const size_t sizeofList, void * dataToCopy, circularAdjListCopier_pf inserterFunction, circularAdjListComparator_pf comparatorFunction )
{
  if( inserterFunction == NULL )
  { fprintf( stderr, "circularAdjList_add need a inserterFunction" ); exit( EXIT_FAILURE );}

  if( circleAdjList == NULL )
  {
    struct AdjList * newAdjList = ( struct AdjList * ) malloc( sizeofList );
    if( newAdjList == NULL ) {perror( "malloc circularAdjList" ); exit( EXIT_FAILURE );}

    //
    if( previous_circleAdjList == NULL )   // n'a jamais était initialisé
    {
      newAdjList->prev = newAdjList;
      newAdjList->next = newAdjList;
    }
    else
    {
      newAdjList->prev = previous_circleAdjList;
      newAdjList->next = previous_circleAdjList->next;
      previous_circleAdjList->next = newAdjList;
      newAdjList->next->prev = newAdjList;
      previous_circleAdjList->isEnd = 0;
    }
    newAdjList->isEnd = 1;

    //
    inserterFunction( newAdjList, dataToCopy );
    return newAdjList;
  }

  //
  if ( comparatorFunction != NULL )
  {
    if( comparatorFunction( circleAdjList, dataToCopy ) == true )
    {
      /** @todo gerer la désallocation */
      inserterFunction( circleAdjList, dataToCopy );
      return circleAdjList;
    }
  }
  else if( !circleAdjList->isEnd )   // pas de comparateur, alors peux directement aller à la fin
  {
    circularAdjList_add_( circleAdjList->prev, NULL, sizeofList, dataToCopy, inserterFunction, comparatorFunction );
    // ce n'est pas important de conaitre le previous ici
    return circleAdjList;
  }

  //
  if( circleAdjList->isEnd ) // ajoute a la fin
    circularAdjList_add_( NULL, circleAdjList, sizeofList, dataToCopy, inserterFunction, comparatorFunction );
  else
    circularAdjList_add_( circleAdjList->next, NULL, sizeofList, dataToCopy, inserterFunction, comparatorFunction );

  //
  return circleAdjList;
}


struct AdjList * circularAdjList_search_( struct AdjList * circularAdjList, void * dataToSearch, circularAdjListComparator_pf comparatorFunction )
{
  if( comparatorFunction == NULL )
  { fprintf( stderr, "circularAdjList_search need a comparator" ); exit( EXIT_FAILURE );}

  //
  if( circularAdjList == NULL )
    return NULL;
  else
  {
    if( comparatorFunction( circularAdjList, dataToSearch ) == true )
      return circularAdjList;
    else if ( !circularAdjList->isEnd )
      return circularAdjList_search_( circularAdjList->next, dataToSearch, comparatorFunction );
  }
  return NULL;
}

struct AdjList * circularAdjList_free_( struct AdjList * circularAdjList, circularAdjListFreer_pf freerFunction )
{
  if( circularAdjList != NULL )
  {
    //
    if ( !circularAdjList->isEnd )
      circularAdjList_free_( circularAdjList->next, freerFunction );

    //
    if( freerFunction != NULL )
      freerFunction( circularAdjList );

    //
    free( circularAdjList );
  }
  return NULL;
}
