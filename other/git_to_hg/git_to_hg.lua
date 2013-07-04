
local commits = {}

function main()
	print('init hg repository. \n')

	create_hg_repository()

	get_all_commits()

	sort_commit()

	print('total commit count: '.. #commits)

	for i,v in ipairs(commits) do
		process_each_commit(v)
	end
end

function create_hg_repository()
	local f = io.open('.hgignore', 'w')
	f:write('syntax: regexp')
	f:write('\r\n')
	f:write('^.git/')
	f:close()

	os.execute('hg init')
end

function sort_commit()
	local commits_sort = {}
	for i, v in ipairs(commits) do
		table.insert(commits_sort, 1, v)
	end

	if #commits ~= #commits_sort then
		print('wrong sort')
	end

	commits = commits_sort
end

function get_note_of_commit(hash)
	local sr = io.popen('git log -n 1 '.. hash)
	local note = ''
	local i = 0

	for line in sr:lines() do
		i = i + 1
		if i > 3 then
			note = note .. '  ' .. line
		end
	end

	sr:close()
	return note
end

function process_each_commit(hash)
	local note = get_note_of_commit(hash)
	
	print('')
	print('************')
	print('[NOTE]   ' .. note)

	os.execute('git reset --hard '..hash)
	os.execute('hg add .')

	local sr = io.popen('hg status')
	for line in sr:lines() do
		file = string.match(line, '! ([^\n]+)')

		if file ~= nil then
			print('[RM] ' .. file)
			os.execute('hg rm ' .. file)			
		end
	end
	sr:close()

	os.execute('hg commit -m "'..note..'"')
end

function get_all_commits()
	local r = io.popen('git log')

	for line in r:lines() do
		hash = string.match(line, 'commit (%w+)')
		if hash ~= nil then
			table.insert(commits, hash)
		end
	end

	r:close()
end

main()