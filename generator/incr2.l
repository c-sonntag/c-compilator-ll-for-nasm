entier $tab[ 10 ];

main() 
entier $i, entier $j;
{
  $i=0;
  tantque $i < 10 faire {
    $tab[$i] = 0;
    $j = 0;
    tantque $j < $i faire {
      incr $tab[$i];
      incr $j;
      }
    ecrire($tab[$i]);
    incr $i;
  }
}
