entier $tab[100];

remplir()
entier $i;
{
    $i = 0;
    tantque $i < 100 faire {
       $tab[ $i ] = $i;
	   $i = $i + 1;
    }           
}

recurse_count(entier $i)
{
    si $i >= 100  alors {
		retour 0;
	} sinon {
		retour $tab[ $i ] + recurse_count($i+1);
	}
}


main()
{
remplir(); 
ecrire( recurse_count(0) );
ecrire( (100 * (0 + 99)) / 2  );
}
