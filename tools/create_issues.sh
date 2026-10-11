#!/usr/bin/env bash
# Crée labels, jalon et issues d'un projet à partir d'un fichier texte.
# Usage : tools/create_issues.sh issues.txt "v0.1.0 MVP"
# Format de issues.txt (une issue par ligne) : label|titre|description   (# = commentaire)
set -euo pipefail
FILE="${1:?fichier d'issues attendu}"
MILESTONE="${2:-v0.1.0 MVP}"
gh api "repos/{owner}/{repo}/milestones" -f title="$MILESTONE" >/dev/null 2>&1 || true
for l in "must:0E8A16" "should:FBCA04" "could:C5DEF5" "security:B60205" "docs:0075CA"; do
  gh label create "${l%%:*}" --color "${l##*:}" --force >/dev/null
done
while IFS='|' read -r label title body; do
  [[ -z "${title:-}" || "$label" == \#* ]] && continue
  gh issue create --title "$title" --body "${body:-}" --label "$label" --milestone "$MILESTONE"
done < "$FILE"
