entier $a, entier $b;

pgcd( entier $a, entier $b )
entier $c;
{
ecrire(123456789);
ecrire($a);
ecrire($b);

$c = ( $a / $b );
ecrire(987654321);

ecrire(( $a / $b ));
ecrire(( $a / $b )* $b);

  $c = $a - ( $a / $b ) * $b;
  
  ecrire($c);
  
  ecrire(123456789);
  si $c = 0 alors{ retour $b; }
  sinon{ retour pgcd( $b, $c ); }
}

main() {
  $a = lire();
  $b = lire();
  si $b < $a alors{ ecrire( pgcd( $a, $b ) ); }
  sinon{ ecrire( pgcd( $b, $a ) ); }
}
