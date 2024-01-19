# Commands
$cmakeCommand = "cmake"
$cmakeArgs = "-G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .."
$cleanCommand = "ninja"
$cleanArgs = "clean"
$buildCommand = "ninja"
$buildArgs = "-d stats"
$runCommand = "cmd.exe"
$runArgs = "/C cd build/output && .\Antares.exe"

# Working directory
$workingDirectory = "build"

# Stats file
$statsFile = "$workingDirectory/build_stats.json"
Write-Host "Stats file path: $statsFile"

# Parse command line arguments
$clean = $args.Contains("-clean")
$run = $args.Contains("-run")

# Read the stats from the file
if (Test-Path $statsFile) {
    $stats = Get-Content $statsFile | ConvertFrom-Json
} else {
    $stats = @{
        "buildCount" = 1
        "totalBuildTime" = 180
    }
}

# Calculate the average build time
$averageBuildTime = if ($stats.buildCount -gt 0) { $stats.totalBuildTime / $stats.buildCount } else { 180 }

# Always run the cmake command to generate the files
Write-Host "Generating files with cmake..."
Start-Process -FilePath $cmakeCommand -ArgumentList $cmakeArgs -NoNewWindow -Wait -WorkingDirectory $workingDirectory

# Perform a clean build if requested
if ($clean) {
    Write-Host "Performing a clean build..."
    Start-Process -FilePath $cleanCommand -ArgumentList $cleanArgs -NoNewWindow -Wait -WorkingDirectory $workingDirectory
}

# Start the build and measure the build time
$buildStart = Get-Date
$buildProcess = Start-Process -FilePath $buildCommand -ArgumentList $buildArgs -NoNewWindow -PassThru -WorkingDirectory $workingDirectory

# Display a progress bar while the build is running
$elapsedTime = 0
$estimatedTimeLeft = $averageBuildTime
$progressBar = $false
$i = 0

while (-not $buildProcess.HasExited) {
    $elapsedTime = (Get-Date) - $buildStart
    $estimatedTimeLeft = ($elapsedTime.TotalSeconds / $i) * ($averageBuildTime - $i)

    if (-not $progressBar) {
        # Clear the console and move the cursor to the top
        Clear-Host
        $progressBar = $true
    }

    # Write the progress bar as the first visible line
    Write-Host "Building..."
    Write-Host ("Elapsed time: $(Get-Date -Date (Get-Date).AddSeconds(-$i) -Format 'hh\:mm\:ss')")
    Write-Host ("Estimated Time Left: $($estimatedTimeLeft.ToString('hh\:mm\:ss'))")
    Write-Host ("Progress: {0}%" -f ([Math]::Min($i / $averageBuildTime * 100, 100)))
    Write-Host ("$i of $averageBuildTime seconds")

    # Wait for the build to finish
    $buildProcess | Wait-Process
    $buildEnd = Get-Date

    # Check if the build was successful
    if ($buildProcess.ExitCode -eq 0) {
        Write-Host "Build successful"

        # Update the stats
        $stats.buildCount++
        $stats.totalBuildTime += ($buildEnd - $buildStart).TotalSeconds

        # Write the stats back to the file
        $stats | ConvertTo-Json | Set-Content $statsFile

        # Run the program if requested
        if ($run) {
            Write-Host "Running $runCommand"
            Start-Process -FilePath $runCommand -NoNewWindow
        }
    } else {
        Write-Host "Build failed"
    }

    Start-Sleep -Milliseconds 500
    $i++
}

# Wait for the build to finish
$buildProcess | Wait-Process
$buildEnd = Get-Date

# Check if the build was successful
if ($buildProcess.ExitCode -eq 0) {
    Write-Host "Build successful"

    # Update the stats
    $stats.buildCount++
    $stats.totalBuildTime += ($buildEnd - $buildStart).TotalSeconds

    # Write the stats back to the file
    $stats | ConvertTo-Json | Set-Content $statsFile

    # Calculate build time in minutes and seconds
    $buildTimeMinutes = [Math]::Floor(($buildEnd - $buildStart).TotalMinutes)
    $buildTimeSeconds = [Math]::Floor(($buildEnd - $buildStart).TotalSeconds) % 60

    # Calculate the percentage difference from the average build time
    $percentageDifference = if ($averageBuildTime -ne 0) { (($buildEnd - $buildStart).TotalSeconds / $averageBuildTime) * 100 } else { 0 }

    # Format the build time and percentage difference
    $formattedBuildTime = "{0:00}:{1:00}" -f $buildTimeMinutes, $buildTimeSeconds
    $formattedPercentageDifference = "{0:0.00}" -f $percentageDifference

    # Output the build stat
    $buildStat = "Build number $($stats.buildCount), took $formattedBuildTime, $formattedPercentageDifference% faster than the average"

    # Output the build stat with colors
    if ($percentageDifference -gt 0) {
        Write-Host $buildStat -ForegroundColor Green
    } elseif ($percentageDifference -lt 0) {
        Write-Host $buildStat -ForegroundColor Red
    } else {
        Write-Host $buildStat
    }

    # Run the program if requested
    if ($run) 
    {
        Write-Host "Running $runCommand"
        Start-Process -FilePath $runCommand -ArgumentList $runArgs -NoNewWindow
    }
} else {
    Write-Host "Build failed"
}