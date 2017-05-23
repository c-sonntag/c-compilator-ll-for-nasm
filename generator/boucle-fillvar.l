entier $tab[ 10 ];

main()
entier $i;
{
    $i = 0;
    tantque $i < 10 faire 
    {
        $tab[ $i ] = $i * $i;
        $i = $i + 1;
    }
	$i = 0;
	tantque $i < 10 faire 
    {
        ecrire( $tab[ $i ] );
        $i = $i + 1;
    }
}

