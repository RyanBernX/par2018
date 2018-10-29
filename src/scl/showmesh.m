function showmesh(file, n)
% n should be 2 or 3

	fid = fopen(file);
	npoint = fscanf(fid, '%d', 1);
	coord = zeros([npoint,n]);
	for i=1:npoint,
		for j=1:n,
			coord(i,j) = fscanf(fid,'%f',1);
		end
	end
	nnode = fscanf(fid, '%d', 1);
	node = zeros([nnode,6]);
	for i=1:nnode,
		for j=1:6,
			node(i,j) = fscanf(fid,'%d',1);
		end
	end
	nside = fscanf(fid,'%d',1);
	side = zeros([nside, 8]);
	for i=1:nside,
		for j=1:8,
			side(i,j) = fscanf(fid,'%d',1);
		end
	end
	x = [coord(node(side(:,3)+1,3)+1,1) coord(node(side(:,4)+1,3)+1,1)];
	y = [coord(node(side(:,3)+1,3)+1,2) coord(node(side(:,4)+1,3)+1,2)];
	if n==3,
		z = [coord(node(side(:,3)+1,3)+1,3) coord(node(side(:,4)+1,3)+1,3)];
		line(x', y', z', 'Color', 'k');
	else
		line(x', y', 'Color', 'k');
	end

