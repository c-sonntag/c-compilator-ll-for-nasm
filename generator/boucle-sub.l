#entier $i, entier $j, entier $v;
entier $i, entier $j;

main()
 entier $v;
{
    $i = 0;
    tantque $i < 10 faire 
    {
        ecrire( 123456789 ); # flag top line
		ecrire( $i );
		$j = 0;
		ecrire( 1234 ); # flag top line
		tantque $j < 10 faire 
		{
			$v = $i * $i * $j;
			ecrire( $v );
			$j = $j + 1;
		}
		$i = $i + 1;
    }
}
