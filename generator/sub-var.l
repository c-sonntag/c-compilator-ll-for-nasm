entier $a;

afficheGlobal()
{
	ecrire(123456789);
	ecrire(10);
	ecrire($a);
}

afficheParam(entier $a)
{
	ecrire(123456789);
	ecrire(30);
	ecrire($a);
}

afficheLocal()
entier $a;
{
	$a = 20;
	ecrire(123456789);
	ecrire(20);
	ecrire($a);
}

main()
{
    $a = 10;
	afficheLocal();
	afficheGlobal();
	afficheParam(30);
}

