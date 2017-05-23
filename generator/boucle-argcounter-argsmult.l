
loop(entier $i, entier $mult) 
{
    tantque $i < 10 faire 
    {
        ecrire( $i * $i * $mult );
        $i = $i + 1;
    }
}

main()
{
    loop( 0 , 1);
    loop( 0 , 2);
}
