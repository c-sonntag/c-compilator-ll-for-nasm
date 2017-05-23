entier $carre, entier $tab[ 10 ];

main()
{
    $tab[ 0 ] = 0;
    tantque $tab[ 0 ] < 10 faire 
    {
        $carre = $tab[ 0 ] * $tab[ 0 ];
        ecrire( $carre );
        $tab[ 0 ] = $tab[ 0 ] + 1;
    }
}
