main()
entier $i;
{
    $i=0;
	ecrire( 123456789 ); # flag top line
	tantque $i < 10 faire 
    {
        si ($i % 2) != 0 alors {
			ecrire( $i );
		} sinon {      
			ecrire( $i - 1 );
		}
		$i = $i + 1;
    }
	$i=0;
	ecrire( 123456789 ); # flag top line
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
