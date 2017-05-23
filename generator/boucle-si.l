main()
entier $i;
{
    $i=0;
	tantque $i < 10 faire 
    {
        si ($i % 2) != 0 alors {
			ecrire( $i );
		} sinon {      
			ecrire( $i - 1 );
		}
		$i = $i + 1;
    }
}
