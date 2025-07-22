# Resume-Selector

This project helps streamline the process of evaluating and ranking candidate resumes based on their CGPA, skills, and project experiences using fundamental data structures in C.

## Project Overview

The Resume Selector is a C-based application that enables recruiters to:

- Input and store candidate details including name, CGPA, skills, and projects.
- Automatically calculate scores based on predefined skill and domain priorities.
- Rank resumes based on total score (skill + project).
- Search for candidates by name or skill.
- Store results in a file (`record.txt`) for future reference.

This project uses **Linked Lists** to dynamically manage resumes and performs various operations efficiently.

---

## Features

- Input candidate resume details interactively.
- Score resumes based on:
  - Skill match with job requirements
  - Project domain relevance
- Search:
  - By candidate name
  - By specific skill
- Rank candidates based on computed total score.
- Store ranked data in a text file (`record.txt`).
- Prevents duplicate skill/project entries.

---

## Tech Stack

- **Language:** C
- **Data Structure Used:** Linked List
- **File Handling:** For persistent data storage (`record.txt`)
- **Platform:** Console application (cross-platform)

---

## How It Works

1. Recruiter enters the number of candidates and their details.
2. For each candidate:
   - CGPA is checked (must be ≥ 8).
   - Skills and projects are entered (duplicates not allowed).
   - Skill scores and project scores are calculated based on predefined job requirements.
3. Resumes are ranked based on total score.
4. Ranked list is displayed and saved to `record.txt`.
5. Recruiter can then search resumes by name or skill from a menu interface.

---

